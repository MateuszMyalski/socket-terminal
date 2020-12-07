#include "main.hpp"

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "basic_commands.hpp"
#include "client_session.hpp"
#include "command_dispacher.hpp"
#include "logger.hpp"
#include "socket_terminal_server.hpp"

// TODO add reconnect
/* Server default params */

std::string server_ip = "127.0.0.1";
unsigned short server_port = 8888;
unsigned short max_peers = 10;

void dispatch_exec_args(int argc, char *argv[]) {
  const char *arg_pattern = "i:p:c";
  for (int opt = getopt(argc, argv, arg_pattern); opt != -1;
       opt = getopt(argc, argv, arg_pattern)) {
    switch (opt) {
      case 'i':
        server_ip = optarg;
        break;
      case 'p':
        server_port = std::stoi(optarg);
        break;
      case 'c':
        color_logs = true;
        break;

      default: /* '?' */
        std::cerr << "Usage " << argv[0] << " [-i IPv4] [-p port] [-c] colors"
                  << std::endl;
        exit(EXIT_FAILURE);
    }
  }
}

void display_live_connections(unsigned short live_sessions) {
  static unsigned short prev_live_connections = 0;
  if (prev_live_connections != live_sessions) {
    prev_live_connections = live_sessions;
    std::stringstream info;
    info << "Live connections: " << prev_live_connections;
    Logger::log("server", info.str(), Logger::WHITE);
  }
}

int main(int argc, char *argv[]) {
  /* Dispatch args */
  dispatch_exec_args(argc, argv);

  /* Assign available commands for clients */
  CommandDispacher *command_dispacher = new CommandDispacher();
  command_dispacher->register_command("echo", new BasicCommands::Echo);
  command_dispacher->register_command("ping", new BasicCommands::Ping);

  /* Instance of the server for main thread*/
  SocketTerminalServer *server = new SocketTerminalServer(
      server_ip, server_port, max_peers, command_dispacher);

  /* Assign commands with shared data */
  BasicCommands::Peers *peer_cmd = new BasicCommands::Peers();
  peer_cmd->assign_server(server);
  command_dispacher->register_command("peers",
                                      static_cast<Command *>(peer_cmd));

  BasicCommands::Msg *msg_cmd = new BasicCommands::Msg();
  msg_cmd->assign_server(server);
  command_dispacher->register_command("msg", static_cast<Command *>(msg_cmd));
  std::stringstream info;
  info << "Hosted on IP: " << server_ip;
  Logger::log(server->name, info.str(), Logger::RED);
  info.str("");
  info << "Running on port: " << server_port;
  Logger::log(server->name, info.str(), Logger::RED);
  info.str("");
  info << "Max connection: " << max_peers;
  Logger::log(server->name, info.str(), Logger::RED);
  info.str("");

  /* Main thread */
  while (1) {
    ClientSession *client = server->check_for_connection();

    display_live_connections(server->get_live_conn_numbers());

    server->close_dead_sessions();
  }

  delete command_dispacher;
  delete server;
  return EXIT_SUCCESS;
}