#include <iostream>
#include <memory>
#include <vector>

#include "commands/basic_cmd.hpp"
#include "server/identity.hpp"
#include "server/socket_server.hpp"
#include "utils/logger.hpp"

using namespace Server;
using namespace Commands;
constexpr char server_ip[] = "127.0.0.1";
constexpr int32_t server_port = 8888;
constexpr int32_t server_max_peers = 5;

int main(int argc, char *argv[]) {
    CommandsMap std_cmd_set;
    std_cmd_set["debug"] =
        std::shared_ptr<Command>{std::make_shared<DebugCmd>()};
    std_cmd_set["dbg"] = std::shared_ptr<Command>{std::make_shared<DebugCmd>()};
    std_cmd_set["exit"] =
        std::shared_ptr<Command>{std::make_shared<Disconnect>()};
    std_cmd_set["help"] = std::shared_ptr<Command>{std::make_shared<Help>()};

    CommandsMap expanded_cmd_set;
    expanded_cmd_set["version"] =
        std::shared_ptr<Command>{std::make_shared<Version>()};
    expanded_cmd_set["dbg"] =
        std::shared_ptr<Command>{std::make_shared<DebugCmd>()};
    expanded_cmd_set["help"] =
        std::shared_ptr<Command>{std::make_shared<Help>()};
    expanded_cmd_set["exit"] =
        std::shared_ptr<Command>{std::make_shared<Disconnect>()};

    std::vector<Identity> identity_list{
        Identity("aaa", "aaa", std_cmd_set),
        Identity("bbb", "bbb", expanded_cmd_set),
        Identity("ccc", "", std_cmd_set)};

    SocketServer my_server(server_max_peers, identity_list);

    my_server.open(const_cast<char *>(server_ip), server_port, IPv::IPv4);

    while (1) {
        my_server.pool_for_peers();
    }
    return 1;
}