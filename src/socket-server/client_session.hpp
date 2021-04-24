#ifndef CLIENT_SESSION_HPP
#define CLIENT_SESSION_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

#include <iostream>
#include <thread>

namespace ClientSession {
// class ClientSession {
//    private:
//     volatile bool sess_alive_ = false;
//     std::thread client_thread_;
//     long no_msg_sec_ = 120;
//     long no_msg_warn_before_ = 30;  // Must be greater than no_msg_sec_

//     std::string prompt = "client> ";

//    public:
//     size_t packet_size = 1024;
//     struct sockaddr_in in_addr;
//     int client_socket;

//     ClientSession(struct sockaddr_in in_addres, const int socket_handler);
//     virtual ~ClientSession() {}

//     bool is_session_status_alive();
//     void thread_wrapper();
//     void start_session();
//     void end_session();
// };
}
#endif