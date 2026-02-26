/**/

#include "tcp.h"
#include "packet.h"
#include <iostream>

void tcp_init(tcp_t *tcp, tcp_state_t init_state)
{
    tcp->state = init_state;
    tcp->snd_seq = 0;
    tcp->rcv_seq = 0;
}

tcp_packet_t tcp_send_syn(tcp_t *tcp)
{
    tcp->state = SYN_SENT;

    tcp_packet_t pkt = packet_create(
        1,  // SYN
        0,  // ACK
        0,  // FIN
        tcp->snd_seq,
        0
    );

    std::cout <<"Send SYN, state -> SYN_SENT" << std::endl;

    return pkt;
}

tcp_packet_t tcp_send_syn_ack(tcp_t *tcp)
{
    tcp->state = SYN_RECEIVED;

    tcp_packet_t pkt = packet_create(
        1,  // SYN
        1,  // ACK
        0,  // FIN
        tcp->snd_seq,
        tcp->rcv_seq + 1
    );

    std::cout << "Send SYN+ACK, state -> SYN_RECEIVED\n" << std::endl;

    return pkt;
}

tcp_packet_t tcp_send_ack(tcp_t *tcp)
{
    tcp_packet_t pkt = packet_create(
        0,
        1,
        0,
        tcp->snd_seq,
        tcp->rcv_seq + 1
    );

    std::cout << "Send ACK\n" << std::endl;

    return pkt;
}

void tcp_process(tcp_t *tcp, tcp_packet_t *pkt)
{
    switch (tcp->state) {

        case LISTEN:
            if (pkt->syn) {
                tcp->state = SYN_RECEIVED;
            }
            break;

        case SYN_SENT:
            if (pkt->syn && pkt->ack) {
                tcp->state = ESTABLISTED;
            }
            break;

        case SYN_RECEIVED:
            if (pkt->ack) {
                tcp->state = ESTABLISTED;
            }
            break;

        case ESTABLISTED:
            if (pkt->fin) {
                tcp->state = CLOSE_WAIT;
            }
            break;

        default:
            break;
    }
}
