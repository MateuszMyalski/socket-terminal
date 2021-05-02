#include "src/server/client_session.hpp"

#include "src/utils/logger.hpp"

using namespace std::chrono;
using namespace Utils;
namespace Server {
ClientSession::ClientSession(std::unique_ptr<InSocketAPI> user_socket,
                             std::vector<Identity> &identity_list)
    : in_socket(std::move(user_socket)),
      keep_session_alive(ATOMIC_FLAG_INIT),
      last_action_t(system_clock::now()),
      identity_list(identity_list) {
    keep_session_alive.test_and_set();
    member_thread = std::thread(&ClientSession::session_function, this);
};

ClientSession::~ClientSession() {
    if (keep_session_alive.test_and_set()) {
        disconnect("Abort.");
    }
};

void ClientSession::schedule_msg(std::string msg) {
    std::scoped_lock lck(mtx_tx_buffer);
    tx_buffer.push(msg);
}

void ClientSession::send_msg(std::string msg) {
    // TODO add prompt new line handling
    in_socket->send_buffer(msg.c_str(), msg.size());
};

void ClientSession::send_scheduled() {
    std::scoped_lock lck(mtx_tx_buffer);
    while (!tx_buffer.empty()) {
        send_msg(tx_buffer.front());
        tx_buffer.pop();
    }
}

time_point<system_clock> ClientSession::get_last_action() {
    std::scoped_lock lck(mtx_last_action_t);
    return time_point_cast<milliseconds>(last_action_t);
}

void ClientSession::session_function() {
    constexpr duration<int, std::milli> refresh_delay(500);
    std::unique_lock<std::mutex> lck_tx_buffer(mtx_tx_buffer, std::defer_lock);

    // last_action_t = system_clock::now();
    schedule_msg("a");
    schedule_msg("b");
    schedule_msg("c");
    schedule_msg("d\n");
    send_scheduled();

    while (1) {
        if (!keep_session_alive.test_and_set()) {
            keep_session_alive.clear();
            return;
        }

        schedule_msg("Update\n");
        send_scheduled();
        std::this_thread::sleep_for(refresh_delay);
    }

    // Ask for password if invalid set dead connection to be removed and finish
    // thread
    // Log password validation
};

void ClientSession::disconnect(std::string reason) {
    std::unique_lock<std::mutex> lck_keep_alive(mtx_tx_buffer, std::defer_lock);

    std::stringstream log_buff;
    log_buff << "Disconnecting client: " << in_socket->repr_ip();
    log_buff << " Reason: " << reason;
    info(log_buff);

    keep_session_alive.clear();
    member_thread.join();

    schedule_msg("No respond. Timeout occured. Disconnecting...");
    send_scheduled();

    in_socket->close_connection();
}
}