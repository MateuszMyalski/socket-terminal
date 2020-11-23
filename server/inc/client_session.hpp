#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>

#include <iostream>
#include <thread>

#include "command_dispacher.hpp"

class ClientSession {
 private:
  // Session variables
  volatile bool sess_alive_ = false;
  std::thread client_thread_;
  long no_msg_sec_ = 60;
  long no_msg_warn_before_ = 5;  // Must be greater than no_msg_sec_

  // Querry parsing variables
  CommandDispacher *cmd_disp_;
  std::string prompt = "client> ";

 public:
  // Client variables
  size_t packet_size = 10;  // 24;
  struct sockaddr_in in_addr;
  int client_socket;

  ClientSession(struct sockaddr_in in_addres, const int socket_handler,
                CommandDispacher *command_dispacher);
  virtual ~ClientSession() {}

  bool is_session_status_alive();
  void thread_wrapper();
  void start_session();
  void end_session();
};
