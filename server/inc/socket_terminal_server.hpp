#pragma once
#include <arpa/inet.h>

#include <iostream>
#include <list>

#include "client_session.hpp"

class SocketTerminalServer {
 private:
  std::string server_ip;
  unsigned short server_port;
  unsigned short max_connections;

  int server_socket_handler;
  std::list<ClientSession *> live_connections;

 public:
  SocketTerminalServer(std::string server_ip, unsigned short server_port,
                       unsigned short max_connections);
  ~SocketTerminalServer();

  ClientSession *check_for_connection();
  unsigned short get_live_connections();
  void close_dead_sessions();
};