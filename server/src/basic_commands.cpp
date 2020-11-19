#include "basic_commands.hpp"

#include <sstream>

#include "logger.hpp"
#include "network_utils.hpp"

void BasicCommands::Echo::execute(std::stringstream *out_stream,
                                  const std::vector<std::string> &args) {
  std::stringstream product;
  std::vector<std::string> msg(args.begin() + 1, args.end());

  for (std::vector<std::string>::const_iterator arg_it = msg.begin();
       arg_it != msg.end(); arg_it++) {
    if (arg_it == msg.begin())
      product << *arg_it;
    else
      product << " " << *arg_it;
  }
  *out_stream << product.str();
}

void BasicCommands::Ping::execute(std::stringstream *out_stream,
                                  const std::vector<std::string> &args) {
  *out_stream << "pong";
}
