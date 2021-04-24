#include "src/network-hal/sockets_api.hpp"

#include <arpa/inet.h>
#include <stdio.h>  // TODO(Mateusz) Change to logger
#include <sys/fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "utils/logger.hpp"

namespace NetworkHal {

struct SocketAPI::socket_adapter {
    int handler;
    IPv ip_version;
};

SocketAPI::SocketAPI()
    : socket_adapter_impl(std::make_unique<socket_adapter>()){};

void SocketAPI::create_socket(IPv ip_version) {
    // TODO(Mateusz) Add IPv6 if
    const int socket_hndl = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_hndl == -1) {
        printf("Error %s\n", strerror(errno));
        // TODO(Mateusz) Change to log error
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
        return;  // TODO(Mateusz) Change to log error
    }

    // TODO(Mateusz) Add IPv6 if
    struct sockaddr_in address = {.sin_family = AF_INET,
                                  .sin_port = htons(port)};
    int status = inet_pton(AF_INET, ip_address, &address.sin_addr);
    if (status < 1) {
        printf("Error %s\n", strerror(errno));
        // TODO(Mateusz) Change to log error
    }

    status = bind(socket_adapter_impl->handler, (struct sockaddr*)&address,
                  (socklen_t)sizeof(address));
    if (status < 0) {
        printf("Error %s\n", strerror(errno));
        // TODO(Mateusz) Change to log error
    }
}

void SocketAPI::listen_socket(int32_t max_connections) {
    if (!is_socket_valid()) {
        return;  // TODO(Mateusz) Change to log error
    }

    if (max_connections == 0) {
        printf("Max connections must be greater than 0.");
        // TODO(Mateusz) Change to log error
    }
    int status = listen(socket_adapter_impl->handler, max_connections);

    if (status == -1) {
        printf("Error %s\n", strerror(errno));
    }
}

void SocketAPI::close_socket() {
    if (fcntl(socket_adapter_impl->handler, F_GETFD) >= 0) {
        close(socket_adapter_impl->handler);
    }
}

void SocketAPI::set_address_reusability(bool option) {
    if (!is_socket_valid()) {
        return;  // TODO(Mateusz) Change to log error
    }
    setsockopt(socket_adapter_impl->handler, SOL_SOCKET, SO_REUSEADDR, &option,
               sizeof(int));
}

void SocketAPI::set_socket_pool(bool option) {
    if (!is_socket_valid()) {
        return;  // TODO(Mateusz) Change to log error
    }

    if (fcntl(socket_adapter_impl->handler, F_GETFL) & O_NONBLOCK) {
        return;
    }

    if (fcntl(socket_adapter_impl->handler, F_SETFL, O_NONBLOCK) < 0) {
        printf("Error %s\n", strerror(errno));
        // TODO(Mateusz) Change to log error
    }
}

void SocketAPI::send_buffer(const char* buffer, int64_t buffer_size) {
    if (!is_socket_valid()) {
        return;  // TODO(Mateusz) Change to log error
    }

    if (send(socket_adapter_impl->handler, buffer, buffer_size, 0) < 0) {
        printf("Error %s\n", strerror(errno));
        // TODO(Mateusz) Change to log error
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
