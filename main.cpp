#include "tcp/state.h"
#include "tcp/tcp.h"
#include "tcp/packet.h"

int main(int argc, char* argv[])
{
    tcp_t client, server;

    tcp_init(&client, CLOSED);
    tcp_init(&server, LISTEN);

    // 客户端发送 SYN
    tcp_packet_t syn = tcp_send_syn(&client);
    tcp_process(&server, &syn);

    // 服务器发送 SYN+ACK
    tcp_packet_t syn_ack = tcp_send_syn_ack(&server);
    tcp_process(&client, &syn_ack);

    // 客户端发送 ACK
    tcp_packet_t ack = tcp_send_ack(&client);
    tcp_process(&server, &ack);

    return 0;
}

