#ifndef NETWORK_HAL_SOCKETS_API_HPP
#define NETWORK_HAL_SOCKETS_API_HPP

#include <stdint.h>

#include <memory>

namespace NetworkHal {

enum class IPv { IPv4, IPv6 };

class SocketAPI {
   private:
    struct socket_adapter;
    std::unique_ptr<socket_adapter> socket_adapter_impl;

   public:
    SocketAPI();
    ~SocketAPI();
    void create_socket(IPv ip_version);
    bool is_socket_valid();
    void bind_socket(const char* ip_address, uint32_t port);
    void listen_socket(int32_t max_connections);
    void set_address_reusability(bool option);
    void close_socket();
    void send_buffer(const char* buffer, int64_t buffer_size);
    void set_socket_pool(bool option);
    void reset_socket();
    void shutdown_socket();

    // std::ostream& operator<<(std::ostream&, const Something)
    // std::ostream& operator>>(std::ostream&, const Something)
};
}
#endif
