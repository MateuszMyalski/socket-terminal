#include "network_utils.hpp"

#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

struct sockaddr_in NetworkUtils::generate_address(std::string ip,
                                                  unsigned short port) {
  struct sockaddr_in address = {.sin_family = AF_INET, .sin_port = htons(port)};
  if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) < 1) {
    perror("inet_pton()");
    exit(EXIT_FAILURE);
  }

  return address;
}

const int NetworkUtils::create_socket(void) {
  const int handler = socket(AF_INET, SOCK_STREAM, 0);
  if (handler == -1) {
    perror("socket()");
    exit(EXIT_FAILURE);
  }

  return handler;
}

void NetworkUtils::bind_to_address(const int socket_handler,
                                   struct sockaddr_in address) {
  NetworkUtils::descriptor_must_valid(socket_handler);

  int status = bind(socket_handler, (struct sockaddr*)&address,
                    (socklen_t)sizeof(address));
  if (status < 0) {
    perror("bind()");
    exit(EXIT_FAILURE);
  }
}

void NetworkUtils::listen_on_socket(const int socket_handler,
                                    unsigned short max_connections) {
  NetworkUtils::descriptor_must_valid(socket_handler);

  if (max_connections == 0) {
    std::cerr << "Max connections must be greater than 0." << std::endl;
    exit(EXIT_FAILURE);
  }
  int status = listen(socket_handler, max_connections);

  if (status == -1) {
    perror("listen()");
    exit(EXIT_FAILURE);
  }
}

void NetworkUtils::set_non_blocking(const int socket_handler) {
  NetworkUtils::descriptor_must_valid(socket_handler);

  // Check if is already non-blocking
  if (fcntl(socket_handler, F_GETFL) & O_NONBLOCK) {
    return;
  }

  if (fcntl(socket_handler, F_SETFL, O_NONBLOCK) < 0) {
    perror("fcntl()");
    exit(EXIT_FAILURE);
  }
}

void NetworkUtils::close_connection(const int socket_handler) {
  if (fcntl(socket_handler, F_GETFD) >= 0) close(socket_handler);
}

bool NetworkUtils::descriptor_must_valid(const int descriptor) {
  if (fcntl(descriptor, F_GETFD) < 0) {
    perror("fcntl()");
    exit(EXIT_FAILURE);
  } else {
    return true;
  }
}