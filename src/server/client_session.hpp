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

#include "src/network-hal/sockets_api.hpp"
#include "src/server/identity.hpp"

using namespace NetworkHal;
namespace Server {
class ClientSession {
   private:
    constexpr static int32_t recv_packet_size = 1024;
    std::unique_ptr<InSocketAPI> in_socket;

    std::mutex mtx_tx_buffer;
    std::queue<std::string> tx_buffer;

    std::mutex mtx_last_action_t;
    std::chrono::time_point<std::chrono::system_clock> last_action_t;

    std::thread member_thread;
    std::atomic_flag keep_session_alive;

    std::vector<Identity> const& identity_list;
    std::vector<Server::Identity>::const_iterator user_identity;

    void update_last_activity();
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

    std::chrono::time_point<std::chrono::system_clock> get_last_action();
    bool is_dead();

    void disconnect(std::string reason);
};
}
#endif