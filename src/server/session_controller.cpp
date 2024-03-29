#include "session_controller.hpp"

#include <thread>

#include "identity.hpp"
#include "utils/logger.hpp"

using namespace Utils;
using namespace std::chrono;
namespace Server {
SessionController::SessionController(size_t max_peers,
                                     std::vector<Identity> const& identity_list)
    : keep_updating(ATOMIC_FLAG_INIT),
      identity_list(identity_list),
      max_peers(max_peers) {
    established_connections.clear();
    start_session_updating();
}

SessionController::~SessionController() {
    stop_session_updating();
    close_sessions();
}

void SessionController::open_session(std::unique_ptr<InSocketAPI> client) {
    auto client_session =
        std::make_unique<ClientSession>(std::move(client), identity_list);
    if (established_connections.size() == max_peers) {
        warning("Reached max sessions number.");
        client_session->schedule_msg("Reached max sessions number.");
        return;
    }

    std::scoped_lock lock(mtx_session_list);
    established_connections.push_back(std::move(client_session));
    info("Session registered");
}

void SessionController::start_session_updating() {
    if (keep_updating.test_and_set() || session_timeout_t == milliseconds(0)) {
        keep_updating.clear();
        return;
    }

    thread_updater =
        std::thread(&SessionController::cyclic_session_updater, this);
}

void SessionController::stop_session_updating() {
    if (keep_updating.test_and_set()) {
        keep_updating.clear();
        return;
    }
    keep_updating.clear();
    thread_updater.join();
}

void SessionController::cyclic_session_updater() {
    std::unique_lock<std::mutex> lock_connection_list(mtx_session_list,
                                                      std::defer_lock);

    while (1) {
        if (!keep_updating.test_and_set()) {
            keep_updating.clear();
            return;
        }

        lock_connection_list.lock();
        auto i = established_connections.begin();
        while (i != established_connections.end()) {
            auto delta_ms = (*i)->user_activity.elapsed_ms();
            if (delta_ms < std::chrono::milliseconds(0)) {
                (*i)->disconnect("Session killed.");
                i = established_connections.erase(i);
            }

            if (delta_ms > session_timeout_t) {
                (*i)->disconnect("Session timeout.");
                i = established_connections.erase(i);
            } else {
                i++;
            }
        }
        lock_connection_list.unlock();

        std::this_thread::sleep_for(session_refresh_delay);
    }
}

void SessionController::close_sessions() {
    std::scoped_lock lock(mtx_session_list);
    for (auto& session : established_connections) {
        auto session_ = std::move(session);
        session->disconnect("Sessions close.");
    }
}
}