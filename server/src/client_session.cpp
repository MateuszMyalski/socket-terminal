#include "client_session.hpp"

#include <string.h>
#include <sys/select.h>

#include <sstream>
#include <vector>

#include "logger.hpp"
#include "network_utils.hpp"

ClientSession::ClientSession(struct sockaddr_in in_addr,
                             const int client_socket,
                             CommandDispacher *command_dispacher)
    : in_addr_(in_addr),
      client_socket_(client_socket),
      cmd_disp_(command_dispacher) {}

void ClientSession::start_session() {
  Logger::log(&in_addr_, "Creating session.", YELLOW);

  sess_alive_ = true;
  client_thread_ = std::thread(&ClientSession::thread_wrapper, this);
  NetworkUtils::send_buffer(prompt, client_socket_);
}

void ClientSession::end_session() {
  Logger::log(&in_addr_, "Closing session.", YELLOW);
  if (sess_alive_) sess_alive_ = false;

  if (client_thread_.joinable()) client_thread_.join();
}

bool ClientSession::is_session_status_alive() { return sess_alive_; }

void ClientSession::thread_wrapper() {
  /* Prepare set fo select */
  fd_set read_fds;
  FD_ZERO(&read_fds);
  FD_SET(client_socket_, &read_fds);

  /* Query binder */
  std::string querry;
  bool query_ready = false;

  /* Thread main loop */
  while (sess_alive_) {
    struct timeval timeout = {no_msg_sec_};
    std::vector<char> buffer(packet_size_);

    /* Set timeout and wait for incoming data */
    const int select_status =
        select(FD_SETSIZE, &read_fds, NULL, NULL, &timeout);

    if (select_status < 0) {
      perror("select()");
      exit(EXIT_FAILURE);
    }
    if (select_status == 0) {
      Logger::log(&in_addr_, "No message received. Session closed.", RED);
      NetworkUtils::close_connection(client_socket_);
      break;
    }

    if (!FD_ISSET(client_socket_, &read_fds)) {
      continue;  // Should never be reached
    }

    int n_recv = recv(client_socket_, &buffer[0], buffer.size(), 0);
    if (n_recv <= 0) {
      perror("recv()");
      exit(EXIT_FAILURE);
    }

    /* Bind query and react to special symbols */
    for (auto &symbol : buffer) {
      switch (symbol) {
        case '\n':
          if (!query_ready) query_ready = true;
          break;

        default:
          if (isprint(symbol)) querry.push_back(symbol);
          break;
      }
    }

    if (!query_ready) continue;

    /* Perform command if query finished */
    std::stringstream cmd_out_stream;
    cmd_disp_->run(&cmd_out_stream, querry);
    if (cmd_out_stream.rdbuf()->in_avail()) {
      Logger::log(&in_addr_, cmd_out_stream.str());
      NetworkUtils::send_buffer(cmd_out_stream.str(), client_socket_);
      NetworkUtils::send_buffer("\n\r", client_socket_);
    }

    NetworkUtils::send_buffer(prompt, client_socket_);
    query_ready = false;
    querry.clear();
  }
  sess_alive_ = false;
}
