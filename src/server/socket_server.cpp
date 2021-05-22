#include "socket_server.hpp"

#include <cstring>

#include "client_session.hpp"
#include "commands/command_dispatcher.hpp"
#include "utils/logger.hpp"

using namespace Utils;
using namespace NetworkHal;
namespace Server {
SocketServer::SocketServer(int32_t max_peers,
                           std::vector<Identity> const& identity_list)
    : max_peers(max_peers), sessions(max_peers, identity_list) {
    if (max_peers < 0) {
        fatal("Server initailzied with invalid param (max_peers).");
        exit(EXIT_FAILURE);
    }
}

void SocketServer::open(char* server_ip, int32_t server_port, IPv ip_version) {
    this->server_ip = server_ip;
    this->server_port = server_port;

    // TODO(Mateusz) add Ipv6

    std::stringstream log_buff;
    log_buff << "Trying to open server on: " << server_ip << ":" << server_port
             << " ...";
    info(log_buff);

    server_socket.reset_socket();
    server_socket.create_socket(ip_version);
    server_socket.bind_socket(server_ip, server_port);
    server_socket.listen_socket(max_peers);
    server_socket.set_socket_no_block();
    server_socket.set_address_reusability(true);

    info("Server opened successfully!");
}

// std::list<ClientSession&> get_session_list() {}
void SocketServer::pool_for_peers() {
    // TODO(Mateusz) Pooling vcersion and no blocking cversion
    std::unique_ptr<InSocketAPI> connection;

    connection = server_socket.accept_connection();
    if (connection != nullptr) {
        std::stringstream log_buff;
        log_buff << "Incoming connection from: " << connection->repr_ip();
        info(log_buff);

        sessions.open_session(std::move(connection));
    }
}
void SocketServer::close() {
    warning("Closing server...");
    sessions.close_sessions();
    server_socket.close_socket();
    server_socket.shutdown_socket();
}
}
