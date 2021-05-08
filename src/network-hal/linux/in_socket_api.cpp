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

void InSocketAPI::send_buffer(const char* in_buffer, int64_t in_buffer_size) {
    if (!is_socket_valid()) {
        Utils::fatal("Invalid socket!");
        return;
    }
    int status =
        send(in_socket_adapter_impl->handler, in_buffer, in_buffer_size, 0);

    if (status < 0) {
        Utils::fatal(strerror(errno));
    }
}

void InSocketAPI::close_connection() {
    if (fcntl(in_socket_adapter_impl->handler, F_GETFD) >= 0) {
        close(in_socket_adapter_impl->handler);
    }
}

template <typename T>
int64_t InSocketAPI::recv_buffer(T& out_buffer) {
    int status = recv(in_socket_adapter_impl->handler, &out_buffer[0],
                      out_buffer.size(), 0);

    if ((status <= -1) && ((errno != EAGAIN) || (errno != EWOULDBLOCK))) {
        Utils::fatal(strerror(errno));
    }

    return status;
}

void InSocketAPI::set_socket_no_block() {
    if (!is_socket_valid()) {
        Utils::fatal("SocketAPI::set_socket_pool: Invalid socket!");
        return;
    }

    if (fcntl(in_socket_adapter_impl->handler, F_GETFL) & O_NONBLOCK) {
        return;
    }

    if (fcntl(in_socket_adapter_impl->handler, F_SETFL, O_NONBLOCK) < 0) {
        Utils::fatal(strerror(errno));
    }
}

template int64_t InSocketAPI::recv_buffer(
    std::array<char, recv_buffer_size_quant>& out_buffer);
template int64_t InSocketAPI::recv_buffer(
    std::array<char, recv_buffer_size_quant << 2>& out_buffer);
template int64_t InSocketAPI::recv_buffer(
    std::array<char, recv_buffer_size_quant << 3>& out_buffer);
template int64_t InSocketAPI::recv_buffer(
    std::array<char, recv_buffer_size_quant << 4>& out_buffer);
}
