#ifndef NETWORK_H
#define NETWORK_H

#include "../core/event_loop.h"
#include "../tcp/packet.h"

class TCP;

class Network {
public:
    Network(EventLoop& loop);

    void connect(TCP* a, TCP* b);
    void send(TCP* from, const Packet& pkt);

private:
    EventLoop& loop;
    TCP* endpoint1 = nullptr;
    TCP* endpoint2 = nullptr;

    uint64_t fixed_delay = 100;
};

#endif
