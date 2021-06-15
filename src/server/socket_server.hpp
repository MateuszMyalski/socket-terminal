#ifndef SOCKET_SERVER_SERVER_HPP
#define SOCKET_SERVER_SERVER_HPP

#include <stdint.h>

#include <list>
#include <string>

#include "identity.hpp"
#include "network-hal/sockets_api.hpp"
#include "session_controller.hpp"

namespace Server {

class SocketServer {
   private:
    char* server_ip;
    int32_t server_port;
    int32_t max_peers;
    NetworkHal::SocketAPI server_socket;
    SessionController sessions;

    void terminate_pending_sessions();

   public:
    std::string name = "server";

    SocketServer(int32_t max_peers, std::vector<Identity> const& identity_list);
    ~SocketServer() = default;

    void open(char* server_ip, int32_t server_port, IPv ip_version);
    void pool_for_peers();
    void update();
    void close();
};

}
#endif
