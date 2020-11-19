#include "client_session.hpp"

#include <string.h>
#include <sys/select.h>

#include <sstream>
#include <vector>

#include "logger.hpp"
#include "network_utils.hpp"

ClientSession::ClientSession(struct sockaddr_in in_address,
                             const int client_socket,
                             CommandDispacher *command_dispacher) {
  this->in_address = in_address;
  this->client_socket = client_socket;
  this->session_alive = false;
  this->repr_ip = inet_ntoa(this->in_address.sin_addr);
  this->command_dispacher = command_dispacher;
}

void ClientSession::start_session() {
  Logger::log(&this->in_address, "Creating session.", YELLOW);

  this->session_alive = true;
  client_thread = std::thread(&ClientSession::thread_wrapper, this);
  NetworkUtils::send_buffer(prompt, this->client_socket);
}

void ClientSession::end_session() {
  Logger::log(&this->in_address, "Closing session.", YELLOW);
  if (this->session_alive) this->session_alive = false;

  if (this->client_thread.joinable()) this->client_thread.join();
}

bool ClientSession::is_session_status_alive() { return this->session_alive; }

void ClientSession::thread_wrapper() {
  fd_set read_fds;
  FD_ZERO(&read_fds);
  FD_SET(this->client_socket, &read_fds);
  std::string querry;
  bool querry_ready = false;

  while (this->session_alive) {
    struct timeval timeout = {this->no_message_sec};
    std::vector<char> buffer(this->max_packet_size);

    const int select_status =
        select(FD_SETSIZE, &read_fds, NULL, NULL, &timeout);

    if (select_status < 0) {
      perror("select()");
      exit(EXIT_FAILURE);
    }
    if (select_status == 0) {
      Logger::log(&this->in_address, "No message received. Session closed.",
                  RED);
      NetworkUtils::close_connection(this->client_socket);
      break;
    }

    if (!FD_ISSET(this->client_socket, &read_fds)) {
      continue; // Should never be reached
    }

    int n_recv = recv(this->client_socket, &buffer[0], buffer.size(), 0);
    if (n_recv <= 0) {
      perror("recv()");
      exit(EXIT_FAILURE);
    }

    for (auto &symbol : buffer) {
      switch (symbol) {
        case '\n':
          if (!querry_ready) querry_ready = true;
          break;

        default:
          if (isprint(symbol)) querry.push_back(symbol);
          break;
      }
    }

    //Logger::log("in buffer", buffer);  // DEBUG
    if (!querry_ready) {
      continue;
    }

    std::stringstream cmd_out_stream;
    this->command_dispacher->run(&cmd_out_stream, querry);
    if (cmd_out_stream.rdbuf()->in_avail()) {
      Logger::log(&this->in_address, cmd_out_stream.str());
      NetworkUtils::send_buffer(cmd_out_stream.str(), this->client_socket);
      NetworkUtils::send_buffer("\n\r", this->client_socket);
    }
    
    NetworkUtils::send_buffer(prompt, this->client_socket);
    querry_ready = false;
    querry.clear();
  }
  this->session_alive = false;
}
