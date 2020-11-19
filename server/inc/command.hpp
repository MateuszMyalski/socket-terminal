#pragma once

#include <iostream>
#include <tuple>
#include <vector>
#include <sstream>

class Command {
 public:
  virtual ~Command() {};
  virtual void execute(std::stringstream *out_stream, const std::vector<std::string> &args) = 0;
};