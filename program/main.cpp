#include <stdio.h>
#include <stdlib.h>

#include "socket-server/server.hpp"
#include "utils/logger.hpp"

int main(int argc, char* argv[]) {
  int a = server::me();
  printf("A: %d\n", a);
  Logger::log("ABC", "AAA", Logger::BLACK);
  return 1;
}