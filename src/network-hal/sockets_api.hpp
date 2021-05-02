#ifndef NETWORK_HAL_SOCKETS_API_HPP
#define NETWORK_HAL_SOCKETS_API_HPP

#include <stdint.h>

#include <memory>

namespace NetworkHal {

enum class IPv { IPv4, IPv6 };

class InSocketAPI {
    friend class SocketAPI;

   private:
    struct in_socket_adapter;
    std::unique_ptr<in_socket_adapter> in_socket_adapter_impl;

   public:
    InSocketAPI();
    ~InSocketAPI();
    std::string repr_ip();
    bool is_socket_valid();
    void send_buffer(const char* buffer, int64_t buffer_size);
    void close_connection();
    // std::ostream& operator<<(std::ostream&, const Something)
    // std::ostream& operator>>(std::ostream&, const Something)
};

class SocketAPI {
   private:
    struct socket_adapter;
    std::unique_ptr<socket_adapter> socket_adapter_impl;

    struct socket_in_adapter;

   public:
    SocketAPI();
    ~SocketAPI();
    bool is_socket_valid();
    void create_socket(IPv ip_version);
    void bind_socket(const char* ip_address, uint32_t port);
    void listen_socket(int32_t max_connections);
    std::unique_ptr<InSocketAPI> accept_connection();
    void set_address_reusability(bool option);
    void close_socket();
    void set_socket_pool(bool option);
    void reset_socket();
    void shutdown_socket();
};

}
#endif
