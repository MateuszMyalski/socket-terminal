#pragma once

#include "command.hpp"
#include "socket_terminal_server.hpp"

namespace BasicCommands {
class Echo;
class Ping;
class Peers;
}  // namespace BasicCommands

class BasicCommands::Echo : public Command {
 public:
  void execute(std::stringstream *out_stream,
               const std::vector<std::string> &args);
};

class BasicCommands::Ping : public Command {
 public:
  void execute(std::stringstream *out_stream,
               const std::vector<std::string> &args);
};

class BasicCommands::Peers : public Command {
 private:
  SocketTerminalServer *server_;

 public:
  void execute(std::stringstream *out_stream,
               const std::vector<std::string> &args);
  void assign_server(SocketTerminalServer *server);
};