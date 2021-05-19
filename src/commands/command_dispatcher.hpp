#ifndef COMMANDS_COMMAND_DISPATCHER_HPP
#define COMMANDS_COMMAND_DISPATCHER_HPP

#include <map>
#include <string>
#include <vector>

#include "command.hpp"

namespace Commands {

class CommandDispatcher {
   private:
    CommandsMap registered_cmds;
    void* ctx;

   public:
    CommandDispatcher(void* ctx);
    ~CommandDispatcher() = default;
    void register_command(const std::string& name,
                          std::shared_ptr<Command> command);
    void run(const std::string& querry);
    void dispatch(const std::vector<std::string>& args);
};
}
#endif