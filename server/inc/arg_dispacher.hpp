#pragma once

#include <vector>

#include "args.hpp"

/*
Args format:
command -s ABC
command --name "ABC bgf"
        ^Name  ^Argument
*/

class ArgDispacher {
 private:
  std::vector<ArgumentBase *> registered_args_;

 public:
  void register_command(ArgumentBase *argument);
  void dispach_args(const std::vector<std::string> &args_list);
};