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

void CommandDispatcher::parse_querry(const std::string& querry,
                                     std::vector<std::string>& args) {
    std::string querry_cpy(querry);
    auto querry_cpy_start = querry_cpy.find_first_not_of(' ');
    if (querry_cpy_start == std::string::npos) {
        return;
    }
    querry_cpy = querry_cpy.substr(querry_cpy_start);

    auto querry_cpy_stop = querry_cpy.find_last_not_of(' ');
    if (querry_cpy_start == std::string::npos) {
        return;
    }
    querry_cpy = querry_cpy.substr(0, querry_cpy_stop + 1);

    std::string arg_tmp;
    arg_tmp.reserve(querry_cpy.size());
    bool skip_space = false;
    bool escape = false;

    for (auto it = querry_cpy.begin(); it != querry_cpy.end(); it++) {
        auto symbol = *it;

        if (symbol == escape_char) {
            if (escape) {
                arg_tmp.push_back(symbol);
                escape = false;
                continue;
            } else {
                escape = true;
                continue;
            }
        }

        if (escape) {
            arg_tmp.push_back(symbol);
            escape = false;
            continue;
        }

        if (symbol == ' ') {
            if (skip_space) {
                arg_tmp.push_back(symbol);
                continue;
            } else {
                args.push_back(arg_tmp);
                arg_tmp.clear();
                continue;
            }
        }

        if (symbol == '"') {
            if (skip_space) {
                skip_space = false;
                continue;
            } else {
                skip_space = true;
                continue;
            }
        }

        arg_tmp.push_back(symbol);

        // escape_next_char = (symbol == escape_char) && !escape_next_char;

        // if (escape_next_char) {
        //     continue;
        // }

        // if (symbol == '"' && !escape_next_char) {
        //     !skip_space;
        //     escape_next_char = false;
        //     continue;
        // }

        // if ((symbol == ' ') && !skip_space && !escape_next_char) {
        //     args.push_back(arg_tmp);
        //     arg_tmp.clear();
        //     escape_next_char = false;
        //     continue;
        // }

        // if (std::next(it) == querry_cpy.end()) {
        //     arg_tmp.push_back(symbol);
        //     args.push_back(arg_tmp);
        //     arg_tmp.clear();
        //     escape_next_char = false;
        //     continue;
        // }

        // arg_tmp.push_back(symbol);
        // escape_next_char = false;
    }

    if (!arg_tmp.empty()) {
        args.push_back(arg_tmp);
        arg_tmp.clear();
    }

    // abc "arg1 lll" "arg2"
    // abc arg1 arg2 "text...."
    // Strip whiespaces
    // Divide into args
    // TODO add " " to use as one arg
    // Rememeber about escape chars
}

void CommandDispatcher::run(const std::string& querry) {
    std::vector<std::string> args;

    parse_querry(querry, args);
}

bool CommandDispatcher::dispatch(const std::vector<std::string>& args) {
    if (args.empty() || registered_cmds.empty()) {
        return false;
    }

    auto cmd_pair = registered_cmds.find(args[0]);
    if (cmd_pair != registered_cmds.end()) {
        cmd_pair->second->execute(ctx, args);
        return true;
    }
    return false;
}
}