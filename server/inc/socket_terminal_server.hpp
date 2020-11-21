#pragma once
#include <arpa/inet.h>

#include <iostream>
#include <list>

#include "client_session.hpp"
#include "command_dispacher.hpp"

class SocketTerminalServer {
 private:
  std::string ip_;
  unsigned short port_;
  unsigned short max_conn_;

  int server_socket_;
  std::list<ClientSession *> live_conn_;

  CommandDispacher *cmd_disp_;

 public:
  std::string name = "server";

  SocketTerminalServer(const std::string &ip, unsigned short port,
                       unsigned short max_connections,
                       CommandDispacher *command_dispacher);
  ~SocketTerminalServer();

  ClientSession *check_for_connection();
  unsigned short get_live_conn_numbers();
  std::list<ClientSession *> get_live_conns();
  void close_dead_sessions();
};