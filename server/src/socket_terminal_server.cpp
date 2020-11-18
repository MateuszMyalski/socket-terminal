#include "socket_terminal_server.hpp"

#include <string.h>
#include <sys/socket.h>

#include <sstream>

#include "logger.hpp"
#include "network_utils.hpp"

SocketTerminalServer::SocketTerminalServer(std::string server_ip,
                                           unsigned short server_port,
                                           unsigned short max_connections) {
  std::stringstream info;
  info << "Hosted on IP: " << server_ip;
  Logger::log("server", info.str(), RED);
  info.str("");
  info << "Running on port: " << server_port;
  Logger::log("server", info.str(), RED);
  info.str("");
  info << "Max connection: " << max_connections;
  Logger::log("server", info.str(), RED);
  info.str("");

  this->server_ip = server_ip;
  this->server_port = server_port;
  this->max_connections = max_connections;

  struct sockaddr_in server_address =
      NetworkUtils::generate_address(this->server_ip, this->server_port);

  this->server_socket_handler = NetworkUtils::create_socket();

  NetworkUtils::bind_to_address(this->server_socket_handler, server_address);
  NetworkUtils::listen_on_socket(this->server_socket_handler,
                                 this->max_connections);
  NetworkUtils::set_non_blocking(this->server_socket_handler);
  NetworkUtils::set_address_reuse(this->server_socket_handler);
}

ClientSession *SocketTerminalServer::check_for_connection() {
  struct sockaddr_in in_address = {0};
  socklen_t address_len = sizeof(in_address);
  
  const int client_socket =
      accept(this->server_socket_handler, (struct sockaddr *)&in_address,
             &address_len);

  if (client_socket < 0 && errno == EWOULDBLOCK) {
    return NULL;
  }
  if (client_socket && errno != EWOULDBLOCK) {
    perror("accept()");
    return NULL;
  }

  Logger::log(&in_address, "Incoming connection", YELLOW);

  ClientSession *client = new ClientSession(in_address, client_socket);
  this->live_connections.push_back(client);
  client->start_session();

  return client;
}

void SocketTerminalServer::close_dead_sessions() {
  std::list<ClientSession *>::iterator conn_iterator =
      this->live_connections.begin();

  while (conn_iterator != this->live_connections.end()) {
    if (!(*conn_iterator)->is_session_status_alive()) {
      (*conn_iterator)->end_session();
      conn_iterator = this->live_connections.erase(conn_iterator);
    }
    conn_iterator++;
  }
}

unsigned short SocketTerminalServer::get_live_connections() {
  return this->live_connections.size();
}

SocketTerminalServer::~SocketTerminalServer() {
  this->close_dead_sessions();
  shutdown(this->server_socket_handler, SHUT_RDWR);
}
