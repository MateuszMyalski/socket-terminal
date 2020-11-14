#pragma once
#include <arpa/inet.h>

#include <iostream>

class NetworkUtils {
 private:
 public:
  NetworkUtils();
  ~NetworkUtils();

  static struct sockaddr_in generate_address(std::string ip,
                                             unsigned short port);
  static const int create_socket(void);
  static void bind_to_address(const int socket_handler, struct sockaddr_in address);
  static void listen_on_socket(const int socket_handler, unsigned short max_connections);
};