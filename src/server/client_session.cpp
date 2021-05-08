#include "src/server/client_session.hpp"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "src/server/input_collector.hpp"
#include "src/utils/logger.hpp"

using namespace std::chrono;
using namespace Utils;
namespace Server {

ClientSession::ClientSession(std::unique_ptr<InSocketAPI> user_socket,
                             std::vector<Identity> const& identity_list)
    : in_socket(std::move(user_socket)),
      keep_session_alive(ATOMIC_FLAG_INIT),
      last_action_t(system_clock::now()),
      identity_list(identity_list),
      user_identity(identity_list.end()) {
    keep_session_alive.test_and_set();
    member_thread = std::thread(&ClientSession::session_function, this);
    in_socket->set_socket_no_block();
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

std::string ClientSession::get_prompt() {
    std::stringstream buff;
    buff << (*user_identity).get_username();
    buff << "> ";

    return buff.str();
};

void ClientSession::send_motd() {
    auto in_time_t = system_clock::to_time_t(last_action_t);
    std::stringstream motd;
    motd
        << "You have connected to the server!\n"
        << "The server time is: "
        << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "\n"
        << "Provide correct identity details to aceess your desired profile.\n";
    schedule_msg(motd.str());
}

void ClientSession::send_raw_msg(std::string msg) {
    in_socket->send_buffer(msg.c_str(), msg.size());
}

void ClientSession::update_last_activity() {
    std::scoped_lock lck_last_action(mtx_last_action_t);
    last_action_t = system_clock::now();
}

void ClientSession::send_scheduled() {
    std::scoped_lock lck(mtx_tx_buffer);
    while (!tx_buffer.empty()) {
        send_raw_msg(tx_buffer.front());
        tx_buffer.pop();
    }
}

time_point<system_clock> ClientSession::get_last_action() {
    std::scoped_lock lck(mtx_last_action_t);
    return time_point_cast<milliseconds>(last_action_t);
}

bool ClientSession::auth() {
    InputConstructor<1024> buffer(in_socket, keep_session_alive);
    constexpr int max_tries = 3;

    schedule_msg("Username: ");
    send_scheduled();
    buffer.pool_for_respond();
    auto username = buffer.get_response_str();
    update_last_activity();

    auto id_it = identity_list.begin();
    for (; id_it != identity_list.end(); id_it++) {
        if ((*id_it).check_username(username)) {
            user_identity = id_it;
        }
    }
    if (user_identity == identity_list.end()) {
        return false;
    }

    auto tries = 0;
    std::string password;
    do {
        schedule_msg("Password: ");
        send_scheduled();
        password = buffer.get_response_str();

        buffer.pool_for_respond();
        update_last_activity();

        if ((*user_identity).check_password(password)) {
            break;
        }
        schedule_msg("Invalid password.");
        tries++;

    } while (tries > max_tries);

    return true;
}

void ClientSession::session_function() {
    constexpr duration<int, std::milli> refresh_delay(500);
    std::unique_lock<std::mutex> lck_tx_buffer(mtx_tx_buffer, std::defer_lock);
    InputConstructor<1024> buffer(in_socket, keep_session_alive);
    std::stringstream tmp_stream;

    send_motd();
    if (!auth()) {
        tmp_stream << in_socket->repr_ip() << " invalid authentication";
        Utils::warning(tmp_stream.str().c_str());
        tmp_stream.str("");

        schedule_msg("Access denied!");
        return;
    }

    tmp_stream << "Access granted! Logged as: "
               << (*user_identity).get_username();
    schedule_msg(tmp_stream.str().c_str());
    tmp_stream.str("");

    tmp_stream << in_socket->repr_ip()
               << " logged as: " << (*user_identity).get_username();
    Utils::info(tmp_stream.str().c_str());
    tmp_stream.str("");

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
    log_buff << "Disconnecting client: " << in_socket->repr_ip()
             << ". Reason: " << reason << "\n";
    info(log_buff);

    keep_session_alive.clear();
    member_thread.join();

    schedule_msg(log_buff.str());
    send_scheduled();

    in_socket->close_connection();
}
}