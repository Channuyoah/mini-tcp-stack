/*
  实现状态机逻辑
*/

#ifndef TCP_H
#define TCP_H

#include "state.h"
#include "packet.h"

typedef struct {
    tcp_state_t state;
    int snd_seq;
    int rcv_seq;
} tcp_t;

void tcp_init(tcp_t *tcp, tcp_state_t init_state);

tcp_packet_t tcp_send_syn(tcp_t *tcp);
tcp_packet_t tcp_send_ack(tcp_t *tcp);
tcp_packet_t tcp_send_syn_ack(tcp_t *tcp);
tcp_packet_t tcp_send_fin(tcp_t *tcp);

void tcp_process(tcp_t *tcp, tcp_packet_t *pkt);

#endif