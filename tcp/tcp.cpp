#include <iostream>
#include "tcp.h"
#include "packet.h"
#include "../network/network.h"

TCP::TCP(std::string name, EventLoop& loop)
    :name(name), loop(loop), state(TcpState::CLOSED)
{
}

void TCP::set_network(Network* net) {
    network = net;
}

void TCP::set_state(TcpState s) {
    state = s;
}

// 主动打开(客户端)
void TCP::passive_open() {
    state = TcpState::LISTEN;
}

void TCP::active_open() {
    std::cout << name << " -> ACTIVE OPEN" << std::endl;

    state = TcpState::SYN_SENT;

    send_seq = 1000;

    Packet pkt;
    pkt.syn = true;
    pkt.seq = send_seq;

    send(pkt);

    start_timer();    // 启动RT0
}

// 发送报文(统一出口)
void TCP::send(const Packet& pkt) {
    std::cout << name << " SEND: "
              << "[SYN=" << pkt.syn
              << " ACK=" << pkt.ack
              << " FIN=" << pkt.fin
              << " SEQ=" << pkt.seq
              << " ACK_SEQ=" << pkt.ack_seq
              << "]" << std::endl;

    if (!network) {
        std::cout << "ERROR: network no set!" << std::endl;
        return;
    }

    network->send(this, pkt);
}
void TCP::send_ack(int ack) {
    Packet pkt;

    pkt.ack = true;
    pkt.ack_seq = ack;

    network.send(pkt);
}

// 发送数据
void TCP::send_data(const std::string& data) {
    if (state != TcpState::ESTABLISHED) {
        std::cout << name << " CONNECTION NOT ESTABLISHED" << std::endl;
        return;
    }

    size_t pos = 0;
    
    while (pos < data.size()) {
        size_t len = std::min(mss, data.size() - pos);

        Packet pkt;
        pkt.seq = send_next;
        pkt.payload = data.substr(pos, len);

        Segment seg;
        seg.pkt = pkt;
        seg.seq_start = send_next;
        seg.seq_end = send_next + len;

        send_buffer.push_back(seq);

        send_next += len;
        pos += len;
    }

    try_send();
}

void TCP::try_send()
{
    uint32_t window = cwnd * mss;

    for (auto& seg : send_buffer)
    {
        uint32_t in_flight = send_next - send_unac;

        if (in_flight >= window)
            break;

        // 已确认
        if (seg.seq_end <= send_unac)
            continue;

        // 已发送
        if (seg.sent)
            continue;

        send(seg.pkt);

        seg.sent = true;

        if (!timer_active)
            start_timer();
    }
}

void TCP::handle_ack(uint32_t ack_seq)
{
    if (ack_seq <= send_unac) {
        return;
    }

    send_unac = ack_seq;

    while (!send_buffer.empty()) {

        if (send_buffer.front().seq_end <= send_unac)
            send_buffer.pop_front();
        else
            break;
    }

    if (send_buffer.empty())
        stop_timer();
    else {
        start_timer();
    }

    try_send();
}

void TCP::handle_data(Packet pkt) {
    if (pkt.seq == expected_seq) {
        deliver(pkt.data);
        expected_seq++;

        while (recv_buffer.count(excpeted_seq)) {
            auto p = recv_buffer[expected_seq];
            recv_buffer.erase(expected_seq);
            deliver(p.data);
            expected_seq++;
        }
    } else if (pkt.seq > expected_seq) {
        recv_buffer[pkt.seq] = pkt;
    }
    send_ack(expected_seq);
}

// 收到报文
void TCP::on_receive(const Packet& pkt) {
    std::cout << name << " RECIVE: "
              << "[SYN=" << pkt.syn
              << " ACK=" << pkt.ack
              << " FIN=" << pkt.fin
              << " SEQ=" << pkt.seq
              << " ACK_SEQ=" << pkt.ack_seq
              << "]" << std::endl;
    
    // 1.处理ACK
    if (pkt.ack) {
        handle_ack(pkt.ack_seq);
    }

    // 2.处理数据
    if (pkt.has_data()) {
        if (pkt.seq == rcv_next) {
            rcv_next += pkt.length();

            Packet ack_pkt;
            ack_pkt.ack = true;
            ack_pkt.ack_seq = rcv_next;

            send(ack_pkt);
        }
        else {
            // 乱序包，重发当前ACK
            Packet ack_pkt;
            ack_pkt.ack = true;
            ack_pkt.ack_seq = rcv_next;

            send(ack_pkt);
        }
    }

    // 3.处理状态机
    switch(state) {
        // 服务器 LISTEN
        case TcpState::LISTEN:
            if (pkt.syn) {
                rcv_seq = pkt.seq;
                send_seq = 2000;  // 随机起始值

                Packet reply;
                reply.syn = true;
                reply.ack = true;
                reply.seq = send_seq;
                reply.ack_seq = rcv_seq + 1;

                state = TcpState::SYN_RECEIVED;
                send(reply);

                start_timer();      // 服务器启动RTO
            }
            break;
        
        // 客户端 SYN_SENT 处理握手
        case TcpState::SYN_SENT:
            if (pkt.syn && pkt.ack) {
                stop_timer();   // 取消RTO

                rcv_seq = pkt.seq;

                Packet reply;
                reply.ack = true;
                send_seq++;
                reply.seq = send_seq;
                reply.ack_seq = rcv_seq + 1;

                state = TcpState::ESTABLISHED;
                send(reply);

                rto = initial_rto;
                retry_count = 0;
                std::cout << name << " -> ESTABLISHED" << std::endl;
            }
            break;

        // 服务器 SN_RECEIVED 处理最后的ACK
        case TcpState::SYN_RECEIVED:
            if (pkt.ack) {
                stop_timer();
                state = TcpState::ESTABLISHED;
                rto = initial_rto;
                retry_count = 0;
                std::cout << name << " -> ESTABLISHED" << std::endl;
            } 
            else if (pkt.syn) {
                // 重复SYN (说明SYN+ACK丢了)
                std::cout << name << " RESEND SYN+ACK" << std::endl;

                Packet reply;
                reply.syn = true;
                reply.ack = true;
                reply.seq = send_seq;
                reply.ack_seq = pkt.seq + 1;

                send(reply);
            }
            break;

        // 已经建立连接了 处理数据 + ACK
        case TcpState::ESTABLISHED:
            if (pkt.has_data()) {
                if (pkt.seq == rcv_next) {
                    rcv_next += pkt.length();
                    send_ack(rcv_next);
                } else {
                    // 乱序 直接发送当前ack
                    send_ack(rcv_next);
                }
            }
            
        default:
            break;
    }
}

void TCP::start_timer() {
    timer_active = true;
    timer_id++;

    uint64_t current_id = timer_id;

    loop.schedule(rto, [this, current_id]() {
        on_timeout(current_id);
    });
}

void TCP::stop_timer() {
    timer_active = false;
}

void TCP::on_timeout(uint64_t id) {
    if (!timer_active || id != timer_id) return;        // 旧timer失效

    std:: cout << name << "  TIMEOUT" << std::endl;

    retry_count++;

    if (retry_count > max_retries) {
        std::cout << name << " CONNECTION FAILED" << std::endl;
        state = TcpState::CLOSED;
        timer_active = false;
        return;
    }

    rto = std::min(rto * 2, max_rto);
    // retransmit() 函数负责重发什么数据(后续补充)

    if (state == TcpState::SYN_SENT) {
        std::cout << name << "  RETRANSMIT SYN" << std::endl;

        Packet pkt;
        pkt.syn = true;
        pkt.seq = send_seq;

        send(pkt);
        start_timer();    // 重启RTO
    }
    else if (state == TcpState::SYN_RECEIVED) {
        std::cout << name << " RETRANSMIT SYN+ACK" << std::endl;

        Packet pkt;
        pkt.syn = true;
        pkt.ack = true;
        pkt.seq = send_seq;
        pkt.ack_seq = rcv_seq + 1;

        send(pkt);
        start_timer();    // 重启RTO
    }

}
