#include "main.hpp"

#include <unistd.h>

#include <iostream>

#include "client_session.hpp"
#include "socket_terminal_server.hpp"


// TODO Standarize logs
// TODO Write to client
// TODo Parser

int main(int argc, char *argv[]) {
  SocketTerminalServer *server =
      new SocketTerminalServer("127.0.0.1", 8888, 10);

  while (1) {
    ClientSession *client = server->check_for_connection();
    if (client == NULL) {
      // std::cout << "Waiting for connection..." << std::endl;
      std::cout << "Live connections: " << server->get_live_connections()
                << std::endl;
    }
    server->close_dead_sessions();
    usleep(1000 * 1000);
  }

  delete server;
  return EXIT_SUCCESS;
}