#include "main.hpp"

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "basic_commands.hpp"
#include "client_session.hpp"
#include "command_dispacher.hpp"
#include "logger.hpp"
#include "socket_terminal_server.hpp"

// TODO perror -> strerror(errno)
// TODO add reconnect
// TODO add args
// Move variable in constructor setting to :

void display_live_connections(unsigned short live_sessions) {
  static unsigned short prev_live_connections = 0;
  if (prev_live_connections != live_sessions) {
    prev_live_connections = live_sessions;
    std::stringstream info;
    info << "Live connections: " << prev_live_connections;
    Logger::log("server", info.str());
  }
}

int main(int argc, char *argv[]) {
  /* Assign available commands for clients */
  CommandDispacher *command_dispacher = new CommandDispacher();
  command_dispacher->register_command("echo", new BasicCommands::Echo);
  command_dispacher->register_command("ping", new BasicCommands::Ping);

  /* Instance of the server for main thread*/
  std::string ip = "127.0.0.1";
  unsigned short port = 8888;
  unsigned short max_conn = 10;
  SocketTerminalServer *server =
      new SocketTerminalServer(ip, port, max_conn, command_dispacher);

  std::stringstream info;
  info << "Hosted on IP: " << ip;
  Logger::log(server->name, info.str(), RED);
  info.str("");
  info << "Running on port: " << port;
  Logger::log(server->name, info.str(), RED);
  info.str("");
  info << "Max connection: " << max_conn;
  Logger::log(server->name, info.str(), RED);
  info.str("");

  /* Main thread */
  while (1) {
    ClientSession *client = server->check_for_connection();

    display_live_connections(server->get_live_connections());

    server->close_dead_sessions();
  }

  delete command_dispacher;
  delete server;
  return EXIT_SUCCESS;
}