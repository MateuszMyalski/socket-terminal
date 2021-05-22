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
    virtual void execute(void* ctx, const std::vector<std::string>& args) {
        auto string_ctx = static_cast<std::string*>(ctx);

        bool is_first = true;
        for (auto& arg : args) {
            if (!is_first) {
                string_ctx->push_back(' ');
            }
            is_first = false;
            string_ctx->append(arg);
        }
    };
};

typedef std::map<std::string, std::shared_ptr<Command>> CommandsMap;
}
#endif