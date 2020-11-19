#pragma once
#include <arpa/inet.h>

#include <iostream>
#include <sstream>

namespace NetworkUtils {

struct sockaddr_in generate_address(std::string ip, unsigned short port);
const int create_socket(void);
void bind_to_address(const int socket_handler, struct sockaddr_in address);
void listen_on_socket(const int socket_handler, unsigned short max_connections);
void set_non_blocking(const int socket_handler);
void close_connection(const int socket_handler);
void set_address_reuse(const int socket_handler);
void send_buffer(const std::string &buffer_string, const int socket_handler);
}
