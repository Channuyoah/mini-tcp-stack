/*
  定义报文结构 tcp建立连接和断开连接时的报文
  tips: 真实的报文格式应该是在RFC793当中
*/

#ifndef PACKET_H
#define PACKET_H

typedef struct {
  int syn;
  int ack;
  int fin;
  
  int seq;
  int ack_seq;
} tcp_packet_t;

tcp_packet_t packet_create(int syn, int ack, int fin,
                           int seq, int ack_seq);

void packet_print(tcp_packet_t *pkt);

#endif
