#include "main.hpp"

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "client_session.hpp"
#include "logger.hpp"
#include "socket_terminal_server.hpp"

// TODO Standarize logs
// TODO Write to client
// TODO mutex and thread to std
// TODO Parser
// TODO perror -> strerror(errno)
// TODO add reconnect

int main(int argc, char *argv[]) {
  SocketTerminalServer *server =
      new SocketTerminalServer("127.0.0.1", 8888, 10);

  unsigned short prev_live_connections = 0;

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

  delete server;
  return EXIT_SUCCESS;
}