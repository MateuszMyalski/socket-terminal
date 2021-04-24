#include "src/socket-server/socket_server.hpp"

namespace SocketServer {
SocketServer::SocketServer(int32_t max_peers,
                           transmission_type server_tx_rx_type)
    //    CommandDispacher& assigned_commands)
    : server_tx_rx_type(server_tx_rx_type),
      max_peers(max_peers),
      socket_hal{0} {
    if (max_peers < 0) {
        // ERROR LOG HERE
        return;
    }

    this->max_peers = max_peers;
}

SocketServer::~SocketServer() { close(); }

server_errors SocketServer::open(char *server_ip, int32_t server_port) {
    this->server_ip = server_ip;
    this->server_port = server_port;

    // TODO(Mateusz) add Ipv6
    SocketsAdapter::create_socket(socket_hal, this->server_ip,
                                  SocketsAdapter::IPv::IPv4);
    SocketsAdapter::bind_socket(socket_hal, server_ip, server_port);
    SocketsAdapter::listen_socket(socket_hal, max_peers);
    SocketsAdapter::set_socket_pool(socket_hal, true);
    SocketsAdapter::set_address_reusability(socket_hal, true);

    // TODO(Mateusz) how to propagate errors?
    return server_errors::ok;
}

// std::list<ClientSession&> get_session_list() {}
void listen() {
    // struct sockaddr_in in_address = {0};
    // socklen_t address_len = sizeof(in_address);

    // const int client_socket =
    //     accept(server_socket_, (struct sockaddr *)&in_address, &address_len);
    // if (client_socket == 0 && errno != EWOULDBLOCK) {
    //     perror("accept()");
    //     return NULL;
    // }

    // /* Connection not established in no polling time */
    // if (client_socket < 0 && errno == EWOULDBLOCK) {
    //     return NULL;
    // }

    // /* Create session for new connection */
    // Logger::log(name, "Incoming connection", Logger::YELLOW);
    // socklen_t in_address_len = sizeof(in_address);
    // int peer_name = getpeername(client_socket, (struct sockaddr
    // *)&in_address,
    //                             &in_address_len);
    // ClientSession *client =
    //     new ClientSession(in_address, client_socket, cmd_disp_);
    // live_conn_.push_back(client);
    // client->start_session();

    // return client;
}
// void update() {}
void SocketServer::close() {
    SocketsAdapter::close_socket(socket_hal);
    SocketsAdapter::shutdown_socket(socket_hal);
}

// void terminate_pending_sessions() {}
}
