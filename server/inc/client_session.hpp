#pragma once

#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>

#include <iostream>

class ClientSession {
 private:
  volatile bool session_alive = false;
  long no_message_sec = 5;
  struct sockaddr_in in_address;
  size_t max_packet_size = 10;  // 1024;
  pthread_mutex_t mutex;
  pthread_t thread_id;
  int client_socket;
  std::string repr_ip;

  void set_sesion_status_alive();
  void connection_thread();

 public:
  ClientSession(struct sockaddr_in in_address, const int client_socket);
  ~ClientSession();

  void start_session();
  void end_session();
  bool is_session_status_alive();
  void terminate_session();
  static void *thread_wrapper(void *context);
};
