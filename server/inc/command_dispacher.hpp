#pragma once

#include <map>
#include <sstream>

#include "command.hpp"

class CommandDispacher {
 public:
  typedef std::map<std::string, Command *> CommandMap;

  void register_command(const std::string &name, Command *command);
  void run(std::stringstream *out_stream, const std::string &unparsed_command_line);
  void dispatch(std::stringstream *out_stream, const std::vector<std::string> &args);

 private:
  CommandMap registered_cmds_;
  const std::string WHITESPACE = " \t\f\v";
};
