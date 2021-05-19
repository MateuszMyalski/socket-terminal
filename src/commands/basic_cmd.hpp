#ifndef BASIC_CMD_HPP
#define BASIC_CMD_HPP

#include "command.hpp"

namespace Commands {
class DebugCmd : public Command {
   public:
    void execute(void* ctx, const std::vector<std::string>& args);
};

class Version : public Command {
   public:
    void execute(void* ctx, const std::vector<std::string>& args);
};
}
#endif