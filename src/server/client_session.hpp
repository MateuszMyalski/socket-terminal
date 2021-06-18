#ifndef CLIENT_SESSION_HPP
#define CLIENT_SESSION_HPP

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "identity.hpp"
#include "network-hal/sockets_api.hpp"
#include "srv_def.hpp"
#include "utils/timer.hpp"

using namespace NetworkHal;
namespace Server {

class ClientSession {
   private:
    std::unique_ptr<InSocketAPI> in_socket;

    std::mutex mtx_tx_buffer;
    std::queue<std::string> tx_buffer;

    std::thread member_thread;
    std::atomic_flag keep_session_alive;

    std::vector<Identity> const& identity_list;

    void send_raw_msg(std::string msg);
    std::string get_input();

    bool auth();
    void send_motd();
    std::string get_prompt();

    bool select_identity(std::string username);
    void session_function();

   public:
    ClientSession(std::unique_ptr<InSocketAPI> user_socket,
                  std::vector<Identity> const& identity_list);
    ~ClientSession();
    void schedule_msg(std::string msg);
    void send_scheduled();

    void disconnect(std::string reason);

    Utils::Timer user_activity;
    std::vector<Server::Identity>::const_iterator user_identity;
};
}
#endif