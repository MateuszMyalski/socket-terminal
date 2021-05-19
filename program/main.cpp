#include <iostream>
#include <memory>
#include <vector>

#include "src/commands/basic_cmd.hpp"
#include "src/server/identity.hpp"
#include "src/server/socket_server.hpp"
#include "src/utils/logger.hpp"

using namespace Server;
using namespace Commands;
constexpr char server_ip[] = "127.0.0.1";
constexpr int32_t server_port = 8888;
constexpr int32_t server_max_peers = 5;

// TODO add cosntexpr to some methods
// TODO template string argumentdst to const char *

int main(int argc, char *argv[]) {
    CommandsMap std_cmd_set;
    std_cmd_set["debug"] =
        std::shared_ptr<Command>{std::make_shared<DebugCmd>()};
    std_cmd_set["dbg"] = std::shared_ptr<Command>{std::make_shared<DebugCmd>()};

    CommandsMap expanded_cmd_set;
    std_cmd_set["version"] =
        std::shared_ptr<Command>{std::make_shared<Version>()};
    std_cmd_set["dbg"] = std::shared_ptr<Command>{std::make_shared<DebugCmd>()};

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