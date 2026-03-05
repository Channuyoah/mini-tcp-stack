#include <iostream>
#include <queue>
#include "tcp/tcp.h"
#include "tcp/packet.h"
#include "network/network.h"
#include "tcp/tcp.h"

int main(int argc, char* argv[]) {
    EventLoop loop;

    TCP client("Client", loop);
    TCP server("Server", loop);

    Network net(loop);

    client.set_network(&net);
    server.set_network(&net);

    net.connect(&client, &server);

    server.set_state(TcpState::LISTEN);

    client.active_open();

    loop.run();

    return 0;
}

