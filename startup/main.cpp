#include <iostream>
#include <memory>
#include <vector>

#include "commands/basic_cmd.hpp"
#include "commands/linux_stats_cmd.hpp"
#include "server/identity.hpp"
#include "server/socket_server.hpp"
#include "utils/logger.hpp"

using namespace Server;
using namespace Commands;
constexpr char server_ip[] = "192.168.0.23";
constexpr int32_t server_port = 8888;
constexpr int32_t server_max_peers = 5;

int main(int argc, char *argv[]) {
    CommandsMap all_cmd_set;
    all_cmd_set["debug"] =
        std::shared_ptr<Command>{std::make_shared<DebugCmd>()};
    all_cmd_set["dbg"] = std::shared_ptr<Command>{std::make_shared<DebugCmd>()};
    all_cmd_set["exit"] =
        std::shared_ptr<Command>{std::make_shared<Disconnect>()};
    all_cmd_set["help"] = std::shared_ptr<Command>{std::make_shared<Help>()};
    all_cmd_set["cpu"] = std::shared_ptr<Command>{std::make_shared<CPUUsage>()};
    all_cmd_set["ram"] = std::shared_ptr<Command>{std::make_shared<RAMUsage>()};

    CommandsMap debug_cmd_set;
    debug_cmd_set["version"] =
        std::shared_ptr<Command>{std::make_shared<Version>()};
    debug_cmd_set["dbg"] =
        std::shared_ptr<Command>{std::make_shared<DebugCmd>()};
    debug_cmd_set["help"] = std::shared_ptr<Command>{std::make_shared<Help>()};
    debug_cmd_set["exit"] =
        std::shared_ptr<Command>{std::make_shared<Disconnect>()};

    CommandsMap cpu_cmd_set;
    cpu_cmd_set["cpu"] = std::shared_ptr<Command>{std::make_shared<CPUUsage>()};
    debug_cmd_set["help"] = std::shared_ptr<Command>{std::make_shared<Help>()};
    cpu_cmd_set["exit"] =
        std::shared_ptr<Command>{std::make_shared<Disconnect>()};

    CommandsMap ram_cmd_set;
    ram_cmd_set["ram"] = std::shared_ptr<Command>{std::make_shared<RAMUsage>()};
    ram_cmd_set["help"] = std::shared_ptr<Command>{std::make_shared<Help>()};
    ram_cmd_set["exit"] =
        std::shared_ptr<Command>{std::make_shared<Disconnect>()};

    std::vector<Identity> identity_list{
        Identity("admin", "12345", all_cmd_set),
        Identity("DBGuser1", "dbg", debug_cmd_set),
        Identity("user2", "User2", ram_cmd_set),
        Identity("user3", "User3", cpu_cmd_set)};

    SocketServer my_server(server_max_peers, identity_list);

    my_server.open(const_cast<char *>(argv[1]), atoi(argv[2]), IPv::IPv4);

    while (1) {
        my_server.pool_for_peers();
    }
    return 1;
}