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
constexpr int32_t server_max_peers = 5;

int main(int argc, char *argv[]) {
    CommandsMap debug_cmd_set;
    debug_cmd_set["version"] =
        std::shared_ptr<Command>{std::make_shared<Version>()};
    debug_cmd_set["dbg"] =
        std::shared_ptr<Command>{std::make_shared<DebugCmd>()};
    debug_cmd_set["help"] = std::shared_ptr<Command>{std::make_shared<Help>()};
    debug_cmd_set["exit"] =
        std::shared_ptr<Command>{std::make_shared<Disconnect>()};

    CommandsMap stat_cmd_set;
    stat_cmd_set["cpu"] =
        std::shared_ptr<Command>{std::make_shared<CPUUsage>()};
    stat_cmd_set["ram"] =
        std::shared_ptr<Command>{std::make_shared<RAMUsage>()};
    stat_cmd_set["help"] = std::shared_ptr<Command>{std::make_shared<Help>()};
    stat_cmd_set["exit"] =
        std::shared_ptr<Command>{std::make_shared<Disconnect>()};

    CommandsMap thermo_cmd_set;
    thermo_cmd_set["help"] = std::shared_ptr<Command>{std::make_shared<Help>()};
    thermo_cmd_set["exit"] =
        std::shared_ptr<Command>{std::make_shared<Disconnect>()};
    thermo_cmd_set["temp0"] =
        std::shared_ptr<Command>{std::make_shared<TempZone>(0)};
    thermo_cmd_set["temp1"] =
        std::shared_ptr<Command>{std::make_shared<TempZone>(1)};
    thermo_cmd_set["temp2"] =
        std::shared_ptr<Command>{std::make_shared<TempZone>(2)};
    thermo_cmd_set["temp3"] =
        std::shared_ptr<Command>{std::make_shared<TempZone>(3)};

    std::vector<Identity> identity_list{
        Identity("dbg", "dbg", debug_cmd_set),
        Identity("stat_user", "stats", stat_cmd_set),
        Identity("thermo_user", "thermo", thermo_cmd_set)};

    SocketServer my_server(server_max_peers, identity_list);

    my_server.open(const_cast<char *>(argv[1]), atoi(argv[2]), IPv::IPv4);

    while (1) {
        my_server.pool_for_peers();
    }
    return 1;
}