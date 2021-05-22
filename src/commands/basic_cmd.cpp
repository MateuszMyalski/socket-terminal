#include "basic_cmd.hpp"

#include "server/client_session.hpp"

namespace Commands {
void DebugCmd::execute(void* ctx, const std::vector<std::string>& args) {
    auto ctx_client = static_cast<Server::ClientSession*>(ctx);
    for (auto& arg : args) {
        ctx_client->schedule_msg(arg);
    }
    ctx_client->send_scheduled();
}

void Version::execute(void* ctx, const std::vector<std::string>& args) {
    auto ctx_client = static_cast<Server::ClientSession*>(ctx);
    ctx_client->schedule_msg("Socket terminal version: 0.2\n");
    ctx_client->schedule_msg(
        "Created by Mateusz Myalski on MIT licence 2021\n");
    ctx_client->send_scheduled();
}
}