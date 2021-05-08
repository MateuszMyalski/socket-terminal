#include <iostream>
#include <vector>

#include "src/server/identity.hpp"
#include "src/server/socket_server.hpp"
#include "src/utils/logger.hpp"

using namespace Server;
constexpr char server_ip[] = "127.0.0.1";
constexpr int32_t server_port = 8888;
constexpr int32_t server_max_peers = 5;

int main(int argc, char* argv[]) {
    std::vector<Identity> identity_list{
        Identity("aaa", "aaa"), Identity("bbb", "bbb"), Identity("ccc", "")};

    SocketServer my_server(server_max_peers, identity_list);
    // TODO add user list with commands
    my_server.open(const_cast<char*>(server_ip), server_port, IPv::IPv4);
    while (1) {
        my_server.pool_for_peers();
    }
    return 1;
}