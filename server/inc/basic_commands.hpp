#pragma once

#include <unistd.h>

#include <sstream>

#include "assert.h"
#include "client_session.hpp"
#include "command.hpp"
#include "network_utils.hpp"
#include "socket_terminal_server.hpp"

namespace BasicCommands {
class Echo;
class Ping;
class Peers;
class Msg;
}  // namespace BasicCommands

class BasicCommands::Echo : public Command {
 public:
  void execute(std::stringstream *out_stream,
               const std::vector<std::string> &args) {
    std::stringstream product;
    std::vector<std::string> msg(args.begin() + 1, args.end());

    for (std::vector<std::string>::const_iterator arg_it = msg.begin();
         arg_it != msg.end(); arg_it++) {
      if (arg_it == msg.begin())
        product << *arg_it;
      else
        product << " " << *arg_it;
    }
    *out_stream << product.str();
  }
};

class BasicCommands::Ping : public Command {
 public:
  void execute(std::stringstream *out_stream,
               const std::vector<std::string> &args) {
    *out_stream << "pong";
  }
};

class BasicCommands::Peers : public Command {
 private:
  SocketTerminalServer *server_;

 public:
  void execute(std::stringstream *out_stream,
               const std::vector<std::string> &args) {
    assert(server_ != NULL);
    std::stringstream product;
    for (const auto &peer : server_->get_live_conns()) {
      product << inet_ntoa(peer->in_addr.sin_addr) << ":"
              << ntohs(peer->in_addr.sin_port) << std::endl;
    }
    *out_stream << product.str();
  }
  void assign_server(SocketTerminalServer *server) {
    assert(server != NULL);
    server_ = server;
  }
};

class BasicCommands::Msg : public Command {
 private:
  SocketTerminalServer *server_;

 public:
  void execute(std::stringstream *out_stream,
               const std::vector<std::string> &args) {
    if (args.size() < 2) {
      *out_stream << "Usage: " << args[0] << " [IPv4] [msg]";
      return;
    }

    /* Generate address structure */
    std::cout << args[1] << std::endl;
    struct sockaddr_in dest_addr = {0};
    try {
      dest_addr = NetworkUtils::generate_address(args[1], 0);
    } catch (const std::exception &e) {
      *out_stream << "Invalid IP.";
      return;
    }

    /* Search for connected peer(s) */
    std::vector<ClientSession *> to_peer;
    for (const auto &peer : server_->get_live_conns()) {
      if (peer->in_addr.sin_addr.s_addr == dest_addr.sin_addr.s_addr) {
        to_peer.push_back(peer);
      }
    }
    if (!to_peer.capacity()) {
      *out_stream << "Peer not found.";
      return;
    }

    /* Prepare msg by binding other args */
    std::stringstream product;
    std::vector<std::string> msg(args.begin() + 2, args.end());

    product << "\r\n";
    for (auto arg_it = msg.begin(); arg_it != msg.end(); arg_it++) {
      if (arg_it == msg.begin())
        product << *arg_it;
      else
        product << " " << *arg_it;
    }
    product << "\r\n";

    /* Send msg to peer(s) */
    for (auto &peer : to_peer) {
      NetworkUtils::send_buffer(product.str(), peer->client_socket);
    }
  }

  void assign_server(SocketTerminalServer *server) {
    assert(server != NULL);
    server_ = server;
  }
};