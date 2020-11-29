#include "arg_dispacher.hpp"

#include <algorithm>
#include <iostream>

void ArgDispacher::register_command(ArgumentBase *argument) {
  if (std::find(registered_args_.begin(), registered_args_.end(), argument) !=
      registered_args_.end())
    throw std::logic_error("Argument already registered.");

  registered_args_.push_back(argument);
}

void ArgDispacher::dispach_args(const std::vector<std::string> &args_list) {
  for (auto &&arg : registered_args_) {
    auto found_arg =
        std::find(args_list.begin(), args_list.end(), arg->get_long_name());
    if (found_arg == std::end(args_list))
      found_arg = std::find(args_list.begin(), std::end(args_list),
                            arg->get_short_name());

    if (found_arg == args_list.end()) continue;

    try {
      arg->set_param(args_list, found_arg);
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }

    // throw std::runtime_error("Invalid option '" + )  TODO! Check what
    // unregistered arg was not use
  }
}