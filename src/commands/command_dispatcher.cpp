#include "command_dispatcher.hpp"

#include "utils/logger.hpp"

namespace Commands {
CommandDispatcher::CommandDispatcher(void* ctx) : ctx(ctx) {}

void CommandDispatcher::register_command(const std::string& name,
                                         std::shared_ptr<Command> command) {
    if (registered_cmds.find(name) == registered_cmds.end()) {
        registered_cmds[name] = command;
    } else {
        Utils::fatal("Cannot add command. Reason: already exists.");
    }
}

void CommandDispatcher::register_multiple_command(
    const CommandsMap& command_map) {
    for (auto& command : command_map) {
        register_command(command.first, command.second);
    }
}

void CommandDispatcher::run(const std::string& querry) {
    std::string args_string(querry);

    auto args_string_start = args_string.find_first_not_of(' ');
    if (args_string_start == std::string::npos) {
        return;
    }
    args_string = args_string.substr(args_string_start);

    auto args_string_stop = args_string.find_last_not_of(' ');
    if (args_string_start == std::string::npos) {
        return;
    }
    args_string = args_string.substr(0, args_string_stop + 1);
    Utils::info(args_string.c_str());

    std::vector<std::string> args;
    std::string arg_tmp(args_string.size(), 0);
    bool skip_space = false;
    for (auto& char_n : args_string) {
        if (char_n == '"') {
            !skip_space;
        }

        if (char_n == ' ' && !skip_space) {
            args.push_back(arg_tmp);
            arg_tmp.clear();
        }
        arg_tmp.push_back(char_n);
    }

    for (auto& arg : args) {
        Utils::info(arg.c_str());
    }

    // abc "arg1 lll" "arg2"
    // abc arg1 arg2 "text...."
    // Strip whiespaces
    // Divide into args
    // TODO add " " to use as one arg
    // Rememeber about escape chars
}

void CommandDispatcher::dispatch(const std::vector<std::string>& args) {
    if (args.empty() || registered_cmds.empty()) {
        return;
    }

    auto cmd_pair = registered_cmds.find(args[0]);
    if (cmd_pair != registered_cmds.end()) {
        cmd_pair->second->execute(ctx, args);
    }
}
}