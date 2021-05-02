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
    constexpr static size_t recv_packet_size = 1024;
    std::unique_ptr<InSocketAPI> in_socket;

    std::mutex mtx_tx_buffer;
    std::queue<std::string> tx_buffer;

    std::mutex mtx_last_action_t;
    std::chrono::time_point<std::chrono::system_clock> last_action_t;

    std::thread member_thread;
    std::atomic_flag keep_session_alive;

    std::vector<Identity>& identity_list;
    Identity user_identity;

    void send_raw_msg(std::string msg);
    void send_motd();
    std::string get_prompt();
    std::string get_input();
    void session_function();
    void update_last_activity();

   public:
    ClientSession(std::unique_ptr<InSocketAPI> user_socket,
                  std::vector<Identity>& identity_list);
    ~ClientSession();
    void schedule_msg(std::string msg);
    std::chrono::time_point<std::chrono::system_clock> get_last_action();
    void send_scheduled();
    void disconnect(std::string reason);
};
}
#endif