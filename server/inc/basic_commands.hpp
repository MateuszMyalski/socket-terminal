#pragma once

#include "command.hpp"

namespace BasicCommands {
class Echo;
class Ping;
}

class BasicCommands::Echo : public Command {
 public:
  void execute(std::stringstream *out_stream, const std::vector<std::string> &args);
};


class BasicCommands::Ping : public Command {
 public:
  void execute(std::stringstream *out_stream, const std::vector<std::string> &args);
};