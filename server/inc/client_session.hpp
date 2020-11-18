#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>

#include <iostream>
#include <thread>

class ClientSession {
 private:
  // Client variables
  size_t max_packet_size = 10;  // 1024;
  struct sockaddr_in in_address;
  std::string repr_ip;
  int client_socket;

  // Session variables
  volatile bool session_alive = false;
  std::thread client_thread;
  long no_message_sec = 5;


 public:
  ClientSession(struct sockaddr_in in_address, const int client_socket);
  ~ClientSession();

  bool is_session_status_alive();
  void thread_wrapper();
  void start_session();
  void end_session();
};
