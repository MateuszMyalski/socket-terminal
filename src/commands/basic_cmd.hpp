#ifndef BASIC_CMD_HPP
#define BASIC_CMD_HPP

#include "command.hpp"

namespace Commands {
class DebugCmd : public Command {
   public:
    void execute(void* ctx, const std::vector<std::string>& args);
    std::string get_man();
};

class Version : public Command {
   public:
    void execute(void* ctx, const std::vector<std::string>& args);
    std::string get_man();
};

class Help : public Command {
   public:
    void execute(void* ctx, const std::vector<std::string>& args);
    std::string get_man();
};

class Disconnect : public Command {
   public:
    void execute(void* ctx, const std::vector<std::string>& args);
    std::string get_man();
};
}
#endif