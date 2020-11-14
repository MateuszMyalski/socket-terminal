#pragma once

#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>

#include <iostream>

class ClientSession {
 private:
  volatile bool session_alive = false;
  struct sockaddr_in in_address;
  pthread_t thread_id;
  int client_socket;
  pthread_mutex_t mutex;
  size_t max_packet_size = 10;//1024;
  std::string repr_ip;


  void connection_thread();

 public:
  ClientSession(struct sockaddr_in in_address, const int client_socket);
  ~ClientSession();

  void start_session();
  void end_session();
  bool is_session_alive();
  static void *thread_wrapper(void *context);
};
