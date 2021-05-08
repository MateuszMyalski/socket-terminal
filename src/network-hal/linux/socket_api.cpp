#include <sys/fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "src/network-hal/linux/adapters_impl.hpp"
#include "src/network-hal/sockets_api.hpp"
#include "utils/logger.hpp"

namespace NetworkHal {
SocketAPI::SocketAPI()
    : socket_adapter_impl(std::make_unique<socket_adapter>()){};
SocketAPI::~SocketAPI() = default;

void SocketAPI::create_socket(IPv ip_version) {
    // TODO(Mateusz) Add IPv6 if
    const int socket_hndl = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_hndl == -1) {
        Utils::fatal(strerror(errno));
    }
    socket_adapter_impl->handler = socket_hndl;
    socket_adapter_impl->ip_version = ip_version;
}

inline bool SocketAPI::is_socket_valid() {
    if (fcntl(socket_adapter_impl->handler, F_GETFD) < 0) {
        return false;
    } else {
        return true;
    }
}

void SocketAPI::bind_socket(const char* ip_address, uint32_t port) {
    if (!is_socket_valid()) {
        Utils::fatal("SocketAPI::bind_socket: Invalid socket!");
        return;
    }
    // TODO(Mateusz) Add IPv6 if
    struct sockaddr_in address = {.sin_family = AF_INET,
                                  .sin_port = htons(port)};
    int status = inet_pton(AF_INET, ip_address, &address.sin_addr);
    if (status < 1) {
        Utils::fatal(strerror(errno));
    }

    status = bind(socket_adapter_impl->handler, (struct sockaddr*)&address,
                  (socklen_t)sizeof(address));
    if (status < 0) {
        Utils::fatal(strerror(errno));
    }
}

void SocketAPI::listen_socket(int32_t max_connections) {
    if (!is_socket_valid()) {
        Utils::fatal("SocketAPI::listen_socket: Invalid socket!");
        return;
    }

    if (max_connections == 0) {
        Utils::fatal(
            "SocketAPI::listen_socket: Max connections must be greater than 0");
    }
    int status = listen(socket_adapter_impl->handler, max_connections);

    if (status == -1) {
        Utils::fatal(strerror(errno));
    }
}

std::unique_ptr<InSocketAPI> SocketAPI::accept_connection() {
    auto in_socket = std::make_unique<InSocketAPI>();
    auto adapter = std::move(in_socket->in_socket_adapter_impl);
    adapter->in_address = {0};
    adapter->address_len = sizeof(adapter->in_address);

    adapter->handler =
        accept(socket_adapter_impl->handler,
               (struct sockaddr*)&adapter->in_address, &adapter->address_len);

    bool is_invalid_status = (adapter->handler == 0) && (errno != EWOULDBLOCK);
    if (is_invalid_status) {
        Utils::fatal(strerror(errno));
        return nullptr;
    }

    bool is_no_connection = (adapter->handler < 0) && (errno == EWOULDBLOCK);
    if (is_no_connection) {
        return nullptr;
    }

    in_socket->in_socket_adapter_impl = std::move(adapter);
    return in_socket;
}

void SocketAPI::close_socket() {
    if (fcntl(socket_adapter_impl->handler, F_GETFD) >= 0) {
        close(socket_adapter_impl->handler);
    }
}

void SocketAPI::set_address_reusability(bool option) {
    if (!is_socket_valid()) {
        Utils::fatal("SocketAPI::set_address_reusability: Invalid socket!");
        return;
    }
    setsockopt(socket_adapter_impl->handler, SOL_SOCKET, SO_REUSEADDR, &option,
               sizeof(int));
}

void SocketAPI::set_socket_no_block() {
    if (!is_socket_valid()) {
        Utils::fatal("SocketAPI::set_socket_pool: Invalid socket!");
        return;
    }

    if (fcntl(socket_adapter_impl->handler, F_GETFL) & O_NONBLOCK) {
        return;
    }

    if (fcntl(socket_adapter_impl->handler, F_SETFL, O_NONBLOCK) < 0) {
        Utils::fatal(strerror(errno));
    }
}

void SocketAPI::shutdown_socket() {
    shutdown(socket_adapter_impl->handler, SHUT_RDWR);
}

void SocketAPI::reset_socket() {
    close_socket();
    shutdown_socket();
    socket_adapter_impl.reset(new socket_adapter);
}

}
