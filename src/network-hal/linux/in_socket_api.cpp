#include <sys/fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "src/network-hal/linux/adapters_impl.hpp"
#include "src/network-hal/sockets_api.hpp"
#include "src/utils/logger.hpp"
namespace NetworkHal {
InSocketAPI::InSocketAPI()
    : in_socket_adapter_impl(
          std::make_unique<InSocketAPI::in_socket_adapter>()){};

InSocketAPI::~InSocketAPI() = default;

inline bool InSocketAPI::is_socket_valid() {
    if (fcntl(in_socket_adapter_impl->handler, F_GETFD) < 0) {
        return false;
    } else {
        return true;
    }
}

std::string InSocketAPI::repr_ip() {
    std::string product;
    product = inet_ntoa(in_socket_adapter_impl->in_address.sin_addr);
    return product;
}

void InSocketAPI::send_buffer(const char* buffer, int64_t buffer_size) {
    if (!is_socket_valid()) {
        Utils::fatal("Invalid socket!");
        return;
    }

    if (send(in_socket_adapter_impl->handler, buffer, buffer_size, 0) < 0) {
        Utils::fatal(strerror(errno));
    }
}

void InSocketAPI::close_connection() {
    if (fcntl(in_socket_adapter_impl->handler, F_GETFD) >= 0) {
        close(in_socket_adapter_impl->handler);
    }
}

}
