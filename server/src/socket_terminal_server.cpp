#include "socket_terminal_server.hpp"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

SocketTerminalServer::SocketTerminalServer(std::string server_ip,
                                           unsigned short server_port,
                                           unsigned short max_connections) {
  /* Sanity check */
  if (max_connections == 0) {
    std::cerr << "Max connections must be greater than 0." << std::endl;
    exit(EXIT_FAILURE);
  }

  /* Debug info */
  std::cout << "Server ip: " << server_ip << std::endl;
  std::cout << "Server port: " << server_port << std::endl;
  std::cout << "Server max connection keep alive: " << max_connections
            << std::endl;

  this->server_ip = server_ip;
  this->server_port = server_port;
  this->max_connections = max_connections;

  struct sockaddr_in server_details = {
      .sin_family = AF_INET,  //(this->use_IPv4) ? AF_INET : AF_INET6,
      .sin_port = htons(this->server_port)};

  switch (
      inet_pton(AF_INET, this->server_ip.c_str(), &server_details.sin_addr)) {
    case -1:
      std::cerr << "Invalid address family." << std::endl;
      exit(EXIT_FAILURE);
      break;

    case 0:
      std::cerr << "Invalid network address." << std::endl;
      exit(EXIT_FAILURE);
      break;
  }

  const int socket_handler = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_handler == -1) {
    std::cout << "Error while opening socket." << std::endl;
    exit(EXIT_FAILURE);
  }

  int bind_status = bind(socket_handler, (struct sockaddr*)&server_details,
                         (socklen_t)sizeof(server_details));
  if (bind_status < 0) {
    std::cerr << "Error while binding to socket. " << strerror(errno)
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

SocketTerminalServer::~SocketTerminalServer() {}
