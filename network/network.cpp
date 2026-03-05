#include <iostream>
#include "network.h"
#include "../tcp/tcp.h"

bool drop_once = true;

Network::Network(EventLoop& l) : loop(l) {}

void Network::connect(TCP* a, TCP* b) {
    endpoint1 = a;
    endpoint2 = b;
}

void Network::send(TCP* from, const Packet& pkt) {
    TCP* target = (from == endpoint1) ? endpoint2 : endpoint1;

    if (pkt.syn && pkt.ack && drop_once) {
      drop_once = false;
      std::cout << "### NETWORK DROPPED SYN+ACK ###" << std::endl;
      return;
    }

    loop.schedule(fixed_delay, [target, pkt]() {
        target->on_receive(pkt);
    });
    std::cout << "Network scheduling delivery\n";
}
