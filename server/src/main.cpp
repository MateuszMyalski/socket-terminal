#include "main.hpp"

#include <iostream>

#include "socket_terminal_server.hpp"

int main(int argc, char *argv[]) {
  SocketTerminalServer *server =
      new SocketTerminalServer("127.0.0.1", 8888, 10);

  while (1) {
  }

  delete server;
  return EXIT_SUCCESS;
}