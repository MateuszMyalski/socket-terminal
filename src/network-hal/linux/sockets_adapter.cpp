#include "src/network-hal/sockets_adapter.hpp"

#include <arpa/inet.h>
#include <stdio.h>  // TODO(Mateusz) Change to logger
#include <sys/fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "utils/logger.hpp"

namespace SocketsAdapter {

void create_socket(socket_adapter& socket_hal, IPv ip_version) {
    // TODO(Mateusz) Add IPv6 if
    const int socket_hndl = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_hndl == -1) {
        printf("Error %s\n", strerror(errno));
        // TODO(Mateusz) Change to log error
    }
    socket_hal.handler = socket_hndl;
    socket_hal.ip_version = ip_version;
}

inline bool is_socket_valid(socket_adapter& socket_hal) {
    if (fcntl(socket_hal.handler, F_GETFD) < 0) {
        return false;
    } else {
        return true;
    }
}

void bind_socket(socket_adapter& socket_hal, const char* ip_address,
                 uint32_t port) {
    if (!is_socket_valid(socket_hal)) {
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

    status = bind(socket_hal.handler, (struct sockaddr*)&address,
                  (socklen_t)sizeof(address));
    if (status < 0) {
        printf("Error %s\n", strerror(errno));
        // TODO(Mateusz) Change to log error
    }
}

void listen_socket(socket_adapter& socket_hal, int32_t max_connections) {
    if (!is_socket_valid(socket_hal)) {
        return;  // TODO(Mateusz) Change to log error
    }

    if (max_connections == 0) {
        printf("Max connections must be greater than 0.");
        // TODO(Mateusz) Change to log error
    }
    int status = listen(socket_hal.handler, max_connections);

    if (status == -1) {
        printf("Error %s\n", strerror(errno));
    }
}

void close_socket(socket_adapter& socket_hal) {
    if (fcntl(socket_hal.handler, F_GETFD) >= 0) {
        close(socket_hal.handler);
    }
}

void set_address_reusability(socket_adapter& socket_hal, bool option) {
    if (!is_socket_valid(socket_hal)) {
        return;  // TODO(Mateusz) Change to log error
    }
    setsockopt(socket_hal.handler, SOL_SOCKET, SO_REUSEADDR, &option,
               sizeof(int));
}

void set_socket_pool(socket_adapter& socket_hal, bool option) {
    if (!is_socket_valid(socket_hal)) {
        return;  // TODO(Mateusz) Change to log error
    }

    if (fcntl(socket_hal.handler, F_GETFL) & O_NONBLOCK) {
        return;
    }

    if (fcntl(socket_hal.handler, F_SETFL, O_NONBLOCK) < 0) {
        printf("Error %s\n", strerror(errno));
        // TODO(Mateusz) Change to log error
    }
}

void send_buffer(socket_adapter& socket_hal, const char* buffer,
                 int64_t buffer_size) {
    if (!is_socket_valid(socket_hal)) {
        return;  // TODO(Mateusz) Change to log error
    }

    if (send(socket_hal.handler, buffer, buffer_size, 0) < 0) {
        printf("Error %s\n", strerror(errno));
        // TODO(Mateusz) Change to log error
    }
}

void shutdown_socket(socket_adapter& socket_hal) {
    shutdown(socket_hal.handler, SHUT_RDWR);
}

}
