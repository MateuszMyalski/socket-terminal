#include "command_dispacher.hpp"

#include <algorithm>
#include <iostream>

#include "logger.hpp"
std::string &ltrim(std::string &str) {
  auto it2 = std::find_if(str.begin(), str.end(), [](char ch) {
    return !std::isspace<char>(ch, std::locale::classic());
  });
  str.erase(str.begin(), it2);
  return str;
}

std::string &rtrim(std::string &str) {
  auto it1 = std::find_if(str.rbegin(), str.rend(), [](char ch) {
    return !std::isspace<char>(ch, std::locale::classic());
  });
  str.erase(it1.base(), str.end());
  return str;
}

void CommandDispacher::register_command(const std::string &name,
                                        Command *command) {
  CommandMap::const_iterator cmd_pair = this->registered_commands.find(name);
  if (cmd_pair != this->registered_commands.end()) {
    // Command already registered
  } else {
    registered_commands[name] = command;
  }
}
void CommandDispacher::dispatch(std::stringstream *out_stream,
                                const std::vector<std::string> &args) {
  if (args.empty()) return;

  CommandMap::const_iterator cmd_pair = this->registered_commands.find(args[0]);

  if (cmd_pair == this->registered_commands.end()) {
    Logger::log("server", args[0] + ": command not found.", YELLOW);
  } else {
    out_stream->str("");
    Command *cmd = cmd_pair->second;
    cmd->execute(out_stream, args);
  }
}

void CommandDispacher::run(std::stringstream *out_stream,
                           const std::string &unparsed_command_line) {
  /* Remove leading and trailing spaces */
  std::string trailed_line(unparsed_command_line);

  size_t start = trailed_line.find_first_not_of(this->WHITESPACE);
  trailed_line = (start == std::string::npos) ? "" : trailed_line.substr(start);

  size_t end = trailed_line.find_last_not_of(this->WHITESPACE);
  trailed_line =
      (end == std::string::npos) ? "" : trailed_line.substr(0, end + 1);

  /* Tokenize line */
  std::vector<std::string> args;
  std::stringstream in_line(trailed_line);
  std::string extracted_arg;

  while (getline(in_line, extracted_arg, ' ')) {
    args.push_back(extracted_arg);
  }

  /* Dispatch */
  this->dispatch(out_stream, args);
}
