#include "network-hal/sockets_api.hpp"

#include <arpa/inet.h>
#include <stdio.h>  // TODO(Mateusz) Change to logger
#include <sys/fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "utils/logger.hpp"

namespace NetworkHal {

struct SocketAPI::socket_adapter {};

SocketAPI::SocketAPI()
    : socket_adapter_impl(std::make_unique<socket_adapter>()){};

void SocketAPI::create_socket(IPv ip_version) {}

inline bool SocketAPI::is_socket_valid() {}

void SocketAPI::bind_socket(const char* ip_address, uint32_t port) {}

void SocketAPI::listen_socket(int32_t max_connections) {}

void SocketAPI::close_socket() {}

void SocketAPI::set_address_reusability(bool option) {}

void SocketAPI::set_socket_pool(bool option) {}

void SocketAPI::send_buffer(const char* buffer, int64_t buffer_size) {}

void SocketAPI::shutdown_socket() {}

void SocketAPI::reset_socket() {
    close_socket();
    shutdown_socket();
    socket_adapter_impl.reset(new socket_adapter);
}
}
