#ifndef NETWORK_HAL_SOCKETS_ADAPTER_HPP
#define NETWORK_HAL_SOCKETS_ADAPTER_HPP
#include <stdint.h>

namespace SocketsAdapter {

enum class IPv { IPv4, IPv6 };
struct socket_adapter {
    int handler;
    IPv ip_version;
};

void create_socket(SocketsAdapter::socket_adapter& socket_hal,
                   const char* ip_address, IPv ip_version);
bool is_socket_valid(SocketsAdapter::socket_adapter& socket_hal);

void bind_socket(SocketsAdapter::socket_adapter& socket_hal,
                 const char* ip_address, uint32_t port);
void listen_socket(SocketsAdapter::socket_adapter& socket_hal,
                   int32_t max_connections);

void set_address_reusability(SocketsAdapter::socket_adapter& socket_hal,
                             bool option);
void close_socket(SocketsAdapter::socket_adapter& socket_hal);
void send_buffer(SocketsAdapter::socket_adapter& socket_hal, const char* buffer,
                 int64_t buffer_size);
void set_socket_pool(SocketsAdapter::socket_adapter& socket_hal, bool option);
void shutdown_socket(SocketsAdapter::socket_adapter& socket_hal);
}
#endif
