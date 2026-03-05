/*
  实现状态机逻辑
*/

#ifndef TCP_H
#define TCP_H

#pragma once
#include <deque>
#include <string>
#include <cstdint>
#include "packet.h"
#include "../core/event_loop.h"

class Network;    // 向前声明

enum class TcpState{
    CLOSED,
    LISTEN,
    SYN_SENT,
    SYN_RECEIVED,
    ESTABLISHED,
    FIN_WAIT_1,
    FIN_WAIT_2,
    CLOSE_WAIT,
    LAST_ACK,
    TIME_WAIT,
};

class TCP {
public:
    TCP(std::string name, EventLoop& loop);

    void set_network(Network* net);
    void set_state(TcpState s);

    void active_open();   // 客户端
    void passive_open();  // 服务器

    // 数据发送接口
    void send_data(const std::string& data);

    // 网络接口
    void on_receive(const Packet& pkt);

private:
    std::string name;
    TcpState state;
    EventLoop& loop;
    Network* network = nullptr;

    uint32_t send_seq = 0;
    uint32_t rcv_seq = 0;
    uint32_t expected_seq = 0;

    // 序号体系
    uint32_t send_unac = 0;    // 最早未确认序号
    uint32_t send_next = 0;    // 下一个可发送序号
    uint32_t rcv_next = 0;     // 期望接收序号

    // 发送窗口参数
    size_t cwnd = 4;        // 固定窗口大小（段数）
    size_t mss  = 4;        // 每段最大数据长度

    uint32_t send_base      // 最早未确认 seq
    uint32_t next_seq       // 下一个发送 seq
    size_t window_size    // 窗口大小

    // 发送缓冲区
    struct Segment {
        Packet pkt;
        uint32_t seq_start;
        uint32_t seq_end;
        bool sent = false;
    };

    std::deque<Segment> send_buffer;    // 存储已发送 但未确认的数据段
    std::map<uint32_t Packet> recv_buffer;

    // 定时器（RTO）
    uint64_t timer_id = 0;
    bool timer_active = false;

    int retry_count = 0;
    int max_retries = 5;

    uint64_t initial_rto = 1000;
    uint64_t rto = 1000;
    uint64_t max_rto = 8000;

private:
    // 内部工具函数
    void send(const Packet& pkt);
    void send_ack(uint32_t ack);
    void start_timer();
    void stop_timer();
    void on_timeout(uint64_t id);

    void try_send();            // 尝试根据窗口发送数据
    void handle_ack(uint32_t ack_seq);
    void handle_data(Packet pkt)
};

#endif
