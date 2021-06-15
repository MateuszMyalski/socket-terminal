#include "basic_cmd.hpp"

#include <sstream>

#include "server/client_session.hpp"
#include "utils/logger.hpp"

namespace Commands {
void DebugCmd::execute(void* ctx, const std::vector<std::string>& args) {
    auto ctx_client = static_cast<Server::ClientSession*>(ctx);
    std::stringstream msg_stream;
    auto arg_cnt = 0;

    for (auto& arg : args) {
        msg_stream.str(std::string());
        msg_stream << "arg[" << arg_cnt << "]: " << arg;
        Utils::info(msg_stream.str().c_str());
        msg_stream << "\n";
        ctx_client->schedule_msg(msg_stream.str());
        arg_cnt++;
    }

    ctx_client->send_scheduled();
}

void Version::execute(void* ctx,
                      [[maybe_unused]] const std::vector<std::string>& args) {
    auto ctx_client = static_cast<Server::ClientSession*>(ctx);
    ctx_client->schedule_msg("Socket terminal version: 0.2\n");
    ctx_client->schedule_msg(
        "Created by Mateusz Myalski on MIT licence 2021\n");
    ctx_client->send_scheduled();
}
}