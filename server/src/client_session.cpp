#include "client_session.hpp"

#include <assert.h>
#include <string.h>
#include <sys/select.h>

#include <sstream>
#include <vector>

#include "logger.hpp"
#include "network_utils.hpp"

ClientSession::ClientSession(struct sockaddr_in in_addres,
                             const int socket_handler,
                             CommandDispacher *command_dispacher)
    : cmd_disp_(command_dispacher),
      in_addr(in_addres),
      client_socket(socket_handler) {}

void ClientSession::start_session() {
  Logger::log(&in_addr, "Creating session.", Logger::YELLOW);

  sess_alive_ = true;
  client_thread_ = std::thread(&ClientSession::thread_wrapper, this);
  NetworkUtils::send_buffer(prompt, client_socket);
}

void ClientSession::end_session() {
  Logger::log(&in_addr, "Closing session.", Logger::YELLOW);

  if (sess_alive_) sess_alive_ = false;
  if (client_thread_.joinable()) client_thread_.join();
}

bool ClientSession::is_session_status_alive() { return sess_alive_; }

void ClientSession::thread_wrapper() {
  /* Special symbols flags */
  bool action_confirm = false;

  /* Waring before timeout logic */
  bool no_msg_warn = true;
  if (no_msg_sec_ <= no_msg_warn_before_) no_msg_warn = false;

  /* Query binder */
  std::string querry;
  bool query_ready = false;

  /* Thread main loop */
  while (sess_alive_) {
    /* Set timeout and wait for incoming data */
    struct timeval timeout = {no_msg_warn ? (no_msg_sec_ - no_msg_warn_before_)
                                          : no_msg_warn_before_};
    std::vector<char> buffer(packet_size);
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(client_socket, &read_fds);

    const int select_status =
        select(FD_SETSIZE, &read_fds, NULL, NULL, &timeout);

    if (select_status < 0) {
      perror("select()");
      exit(EXIT_FAILURE);
    }
    if (select_status == 0 && !no_msg_warn) {
      Logger::log(&in_addr, "No message received. Session closed.", Logger::RED);
      NetworkUtils::close_connection(client_socket);
      break;
    }
    if (select_status == 0 && no_msg_warn) {
      no_msg_warn = false;
      std::stringstream info;
      info << "\r\nNo message for " << no_msg_sec_ - no_msg_warn_before_;
      info << "s disconnection in " << no_msg_warn_before_ << "s.\r\n";
      NetworkUtils::send_buffer(info.str(), client_socket);
      NetworkUtils::send_buffer(prompt, client_socket);
      continue;
    }

    assert(FD_ISSET(client_socket, &read_fds));

    int n_recv = recv(client_socket, &buffer[0], buffer.size(), 0);
    if (n_recv == 0) {
      Logger::log(&in_addr, "Session closed by client.", Logger::WHITE);
      NetworkUtils::close_connection(client_socket);
      break;
    }

    /* Bind query and react to special symbols */
    for (std::vector<char>::const_iterator symbol_it = buffer.begin();
         symbol_it != buffer.end(); symbol_it++) {
      switch (*symbol_it) {
        case '\n':  // New line
          if (!query_ready) query_ready = true;
          break;

        case -1:
          std::cout << "catched" << std::endl;
          break;

        default:
          if (isprint(*symbol_it)) querry.push_back(*symbol_it);
          break;
      }
    }

    Logger::log("Debug", buffer, Logger::WHITE);
    if (!query_ready) continue;

    /* Perform command if query finished */
    std::stringstream cmd_out_stream;
    cmd_disp_->run(&cmd_out_stream, querry);
    if (cmd_out_stream.rdbuf()->in_avail()) {
      Logger::log(&in_addr, cmd_out_stream.str(), Logger::WHITE);
      NetworkUtils::send_buffer(cmd_out_stream.str(), client_socket);
      NetworkUtils::send_buffer("\n\r", client_socket);
    }

    NetworkUtils::send_buffer(prompt, client_socket);
    if (no_msg_sec_ > no_msg_warn_before_) no_msg_warn = true;
    query_ready = false;
    querry.clear();
  }

  /* Session thread is finished */
  sess_alive_ = false;
}
