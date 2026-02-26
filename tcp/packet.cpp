#include <iostream>
#include "packet.h"

tcp_packet_t packet_create(int syn, int ack, int fin,
                               int seq, int ack_seq)
{
    tcp_packet_t pkt;
    pkt.syn = syn;
    pkt.ack = ack;
    pkt.fin = fin;
    pkt.seq = seq;
    pkt.ack_seq = ack_seq;
    
    return pkt;
}

void packet_print(tcp_packet_t *pkt) 
{
   std::cout << "[Packet] SYN=" << pkt->syn
             << " ACK=" << pkt->ack
             << " FIN=" << pkt->fin
             << " SEQ=" << pkt->seq
             << " ACK_SEQ=" << pkt->ack_seq
             << std::endl;
}
