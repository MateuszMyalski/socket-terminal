#include "session_controller.hpp"

#include <thread>

#include "identity.hpp"
#include "utils/logger.hpp"

using namespace Utils;
using namespace std::chrono;
namespace Server {
namespace {
auto calc_time_delta(time_point<system_clock> time_point_ms) {
    auto now_t = time_point_cast<milliseconds>(system_clock::now());
    auto delta_ms = now_t - time_point_ms;

    return delta_ms;
}
}

SessionController::SessionController(int32_t max_peers,
                                     std::vector<Identity> const& identity_list)
    : max_peers(max_peers),
      identity_list(identity_list),
      keep_updating(ATOMIC_FLAG_INIT) {
    established_connections.clear();
    start_session_updating();
};

SessionController::~SessionController() {
    stop_session_updating();
    close_sessions();
};

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
};

void SessionController::start_session_updating() {
    if (keep_updating.test_and_set() || session_timeout_t == milliseconds(0)) {
        keep_updating.clear();
        return;
    }

    thread_updater =
        std::thread(&SessionController::cyclic_session_updater, this);
};

void SessionController::stop_session_updating() {
    if (keep_updating.test_and_set()) {
        keep_updating.clear();
        return;
    }
    keep_updating.clear();
    thread_updater.join();
};

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
            if ((*i)->is_dead()) {
                i = established_connections.erase(i);
                continue;
            }

            auto delta_ms = calc_time_delta((*i)->get_last_action());
            if (delta_ms > session_timeout_t) {
                (*i)->disconnect("Session timeout.");
                i = established_connections.erase(i);
            } else {
                i++;
            }
        }
        // TODO check if not killed thread
        lock_connection_list.unlock();

        std::this_thread::sleep_for(session_refresh_delay);
    }
};

void SessionController::close_sessions() {
    std::scoped_lock lock(mtx_session_list);
    for (auto& session : established_connections) {
        auto session_ = std::move(session);
        session->disconnect("Sessions close.");
    }
};
}