#include "client_session.hpp"

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "commands/basic_cmd.hpp"
#include "commands/command_dispatcher.hpp"
#include "input_collector.hpp"
#include "srv_def.hpp"
#include "utils/logger.hpp"

using namespace Commands;
using namespace std::chrono;
using namespace Utils;
namespace Server {
ClientSession::ClientSession(std::unique_ptr<InSocketAPI> user_socket,
                             std::vector<Identity> const& identity_list)
    : in_socket(std::move(user_socket)),
      keep_session_alive(ATOMIC_FLAG_INIT),
      identity_list(identity_list),
      user_activity(),
      user_identity(identity_list.end()) {
    keep_session_alive.test_and_set();
    member_thread = std::thread(&ClientSession::session_function, this);
    in_socket->set_socket_no_block();
}

ClientSession::~ClientSession() {
    if (keep_session_alive.test_and_set()) {
        disconnect("Abort.");
    }
}

void ClientSession::schedule_msg(std::string msg) {
    std::scoped_lock lck(mtx_tx_buffer);
    tx_buffer.push(msg);
}

std::string ClientSession::get_prompt() {
    std::stringstream buff;
    buff << (*user_identity).get_username();
    buff << "$ ";

    return buff.str();
}

void ClientSession::send_motd() {
    auto in_time_t =
        system_clock::to_time_t(user_activity.get_current_time_point());
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

void ClientSession::send_scheduled() {
    std::scoped_lock lck(mtx_tx_buffer);
    while (!tx_buffer.empty()) {
        send_raw_msg(tx_buffer.front());
        tx_buffer.pop();
    }
}

bool ClientSession::select_identity(std::string username) {
    auto id_it = identity_list.begin();
    for (; id_it != identity_list.end(); id_it++) {
        if ((*id_it).check_username(username)) {
            user_identity = id_it;
            return true;
        }
    }

    return false;
}

bool ClientSession::auth() {
    InputConstructor<recv_packet_size> buffer(in_socket, keep_session_alive);

    schedule_msg("Username: ");
    send_scheduled();
    buffer.pool_for_respond();
    if (!select_identity(buffer.get_response_str())) {
        return false;
    }
    user_activity.update_time_point();

    if ((*user_identity).check_password("")) {
        return true;
    }

    std::string password;
    for (auto tries = 0; tries < invalid_auth_max_tries; tries++) {
        schedule_msg("Password: ");
        send_scheduled();
        buffer.pool_for_respond();
        user_activity.update_time_point();
        password = buffer.get_response_str();
        user_activity.update_time_point();

        if ((*user_identity).check_password(password)) {
            return true;
        }

        schedule_msg("Invalid password.\n");
    }

    return false;
}

void ClientSession::session_function() {
    InputConstructor<recv_packet_size> buffer(in_socket, keep_session_alive);
    std::stringstream tmp_stream;
    send_motd();
    if (!auth()) {
        tmp_stream << in_socket->repr_ip() << " invalid authentication";
        Utils::warning(tmp_stream.str().c_str());
        tmp_stream.str(std::string());

        schedule_msg("Access denied!");
        user_activity += Server::session_timeout_t;
        return;
    }

    tmp_stream << "Access granted! Logged as "
               << (*user_identity).get_username() << "\n";
    schedule_msg(tmp_stream.str().c_str());
    tmp_stream.str(std::string());

    tmp_stream << in_socket->repr_ip()
               << " logged as: " << (*user_identity).get_username();
    info(tmp_stream.str().c_str());
    tmp_stream.str(std::string());
    send_scheduled();

    CommandDispatcher dispatcher(this);
    dispatcher.register_multiple_command(user_identity->get_command_map());

    while (1) {
        if (!keep_session_alive.test_and_set()) {
            keep_session_alive.clear();
            return;
        }

        tmp_stream << get_prompt();
        schedule_msg(tmp_stream.str().c_str());
        tmp_stream.str(std::string());
        send_scheduled();
        buffer.pool_for_respond();
        user_activity.update_time_point();

        if (!dispatcher.run(buffer.get_response_str()) &&
            buffer.get_response_str() != "") {
            schedule_msg("Command not found!\n");
        }
        send_scheduled();
    }
}

void ClientSession::disconnect(std::string reason) {
    std::stringstream log_buff;
    log_buff << "Disconnecting client: " << in_socket->repr_ip()
             << ". Reason: " << reason;
    info(log_buff);

    keep_session_alive.clear();
    member_thread.join();

    log_buff << "\n";
    schedule_msg("\n");
    schedule_msg(log_buff.str());
    send_scheduled();

    in_socket->close_connection();
}
}