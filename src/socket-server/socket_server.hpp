#ifndef SOCKET_SERVER_SERVER_HPP
#define SOCKET_SERVER_SERVER_HPP

#include <stdint.h>

#include <list>
#include <string>

#include "src/commands/commands_dispatcher.hpp"
#include "src/network-hal/sockets_api.hpp"
#include "src/socket-server/client_session.hpp"

namespace SocketServer {
enum class transmission_type { binary, ascii, user_select };
enum class server_errors { ok, error };

class SocketServer {
   private:
    char* server_ip;
    int32_t server_port;
    int32_t max_peers;
    NetworkHal::SocketAPI server_socket;
    transmission_type server_tx_rx_type;

    // std::list<ClientSession&> live_peers;

    // CommandDispacher* assigned_commands;

    void terminate_pending_sessions();

   public:
    std::string name = "server";

    SocketServer(int32_t max_peers, transmission_type server_tx_rx_type);
    //  CommandDispacher& assigned_commands);
    ~SocketServer();

    // std::list<ClientSession&> get_session_list();
    server_errors open(char* server_ip, int32_t server_port);
    void listen();
    void update();
    void close();
};

}
#endif
