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
// Move variable in constructor setting to :

int main(int argc, char *argv[]) {
  unsigned short prev_live_connections = 0;
  CommandDispacher *command_dispacher = new CommandDispacher();
  command_dispacher->register_command("echo", new BasicCommands::Echo);
  command_dispacher->register_command("ping", new BasicCommands::Ping);
  SocketTerminalServer *server =
      new SocketTerminalServer("127.0.0.1", 8888, 10, command_dispacher);

  while (1) {
    ClientSession *client = server->check_for_connection();

    if (prev_live_connections != server->get_live_connections()) {
      prev_live_connections = server->get_live_connections();
      std::stringstream info;
      info << "Live connections: " << prev_live_connections;
      Logger::log("server", info.str());
    }
    server->close_dead_sessions();
    usleep(1000 * 1000);
  }

  delete command_dispacher;
  delete server;
  return EXIT_SUCCESS;
}