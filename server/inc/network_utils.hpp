#pragma once
#include <arpa/inet.h>

#include <iostream>

class NetworkUtils {
 private:
  static bool descriptor_must_valid(const int descriptor);

 public:
  NetworkUtils(){};
  virtual ~NetworkUtils();

  static struct sockaddr_in generate_address(std::string ip,
                                             unsigned short port);
  static const int create_socket(void);
  static void bind_to_address(const int socket_handler,
                              struct sockaddr_in address);
  static void listen_on_socket(const int socket_handler,
                               unsigned short max_connections);
  static void set_non_blocking(const int socket_handler);
  static void close_connection(const int socket_handler);
};