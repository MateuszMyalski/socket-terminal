#ifndef COMMANDS_COMMAND_DISPATCHER_HPP
#define COMMANDS_COMMAND_DISPATCHER_HPP

#include <map>
#include <string>
#include <vector>

#include "command.hpp"

namespace Commands {
constexpr char escape_char = '\\';

class CommandDispatcher {
   private:
    CommandsMap registered_cmds;
    void* ctx;

   public:
    CommandDispatcher(void* ctx);
    ~CommandDispatcher() = default;
    void register_command(const std::string& name,
                          std::shared_ptr<Command> command);
    void register_multiple_command(const CommandsMap& command_map);
    void run(const std::string& querry);
    void parse_querry(const std::string& querry,
                      std::vector<std::string>& args);
    bool dispatch(const std::vector<std::string>& args);
};
}
#endif