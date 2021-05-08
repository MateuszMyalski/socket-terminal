#ifndef NETWORK_HAL_LINUX_HPP
#define NETWORK_HAL_LINUX_HPP
#include <arpa/inet.h>

#include "src/network-hal/sockets_api.hpp"
namespace NetworkHal {
struct SocketAPI::socket_adapter {
    int handler;
    IPv ip_version;
};

struct InSocketAPI::in_socket_adapter {
    int handler;
    struct sockaddr_in in_address;
    socklen_t address_len;
};
}

#endif