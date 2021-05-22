#ifndef COMMAND_COMMAND_HPP
#define COMMAND_COMMAND_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Commands {

class Command {
   public:
    virtual ~Command(){};
    virtual void execute(void* ctx, const std::vector<std::string>& args) = 0;
};

typedef std::map<std::string, std::shared_ptr<Command>> CommandsMap;
}
#endif