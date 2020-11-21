#include "socket_terminal_server.hpp"

#include <string.h>
#include <sys/socket.h>

#include <sstream>

#include "logger.hpp"
#include "network_utils.hpp"

SocketTerminalServer::SocketTerminalServer(const std::string &ip,
                                           unsigned short port,
                                           unsigned short max_connections,
                                           CommandDispacher *command_dispacher)
    : ip_(ip),
      port_(port),
      max_conn_(max_connections),
      cmd_disp_(command_dispacher) {
  /* Server set up */
  struct sockaddr_in server_address =
      NetworkUtils::generate_address(ip_, port_);
  server_socket_ = NetworkUtils::create_socket();

  /* Server parameters */
  NetworkUtils::bind_to_address(server_socket_, server_address);
  NetworkUtils::listen_on_socket(server_socket_, max_conn_);
  NetworkUtils::set_non_blocking(server_socket_);
  NetworkUtils::set_address_reuse(server_socket_);
}

ClientSession *SocketTerminalServer::check_for_connection() {
  struct sockaddr_in in_address = {0};
  socklen_t address_len = sizeof(in_address);

  const int client_socket =
      accept(server_socket_, (struct sockaddr *)&in_address, &address_len);
  if (client_socket == 0 && errno != EWOULDBLOCK) {
    perror("accept()");
    return NULL;
  }

  /* Connection not established in no polling time */
  if (client_socket < 0 && errno == EWOULDBLOCK) {
    return NULL;
  }

  /* Create session for new connection */
  Logger::log(name, "Incoming connection", YELLOW);
  socklen_t in_address_len = sizeof(in_address);
  int peer_name = getpeername(client_socket, (struct sockaddr *) &in_address, &in_address_len);
  ClientSession *client =
      new ClientSession(in_address, client_socket, cmd_disp_);
  live_conn_.push_back(client);
  client->start_session();

  return client;
}

void SocketTerminalServer::close_dead_sessions() {
  std::list<ClientSession *>::iterator conn_it = live_conn_.begin();

  /* Iterate through tracked session and look for dead session */
  while (conn_it != live_conn_.end()) {
    if (!(*conn_it)->is_session_status_alive()) {
      (*conn_it)->end_session();
      delete (*conn_it);
      conn_it = live_conn_.erase(conn_it);
    }
    conn_it++;
  }
}

unsigned short SocketTerminalServer::get_live_conn_numbers() {
  return live_conn_.size();
}

std::list<ClientSession *> SocketTerminalServer::get_live_conns() {
  return live_conn_;
}

SocketTerminalServer::~SocketTerminalServer() {
  this->close_dead_sessions();
  shutdown(server_socket_, SHUT_RDWR);
}
