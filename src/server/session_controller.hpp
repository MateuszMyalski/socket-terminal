#ifndef SERVER_SESSION_CONTROLLER_HPP
#define SERVER_SESSION_CONTROLLER_HPP

#include <atomic>
#include <list>
#include <mutex>
#include <string>

#include "client_session.hpp"
#include "identity.hpp"
#include "src/network-hal/sockets_api.hpp"

using namespace NetworkHal;
namespace Server {
class SessionController {
   private:
    static void client_session(std::unique_ptr<InSocketAPI> user_socket);
    std::list<std::unique_ptr<ClientSession>> established_connections;
    std::atomic_flag keep_updating;
    std::vector<Identity> const& identity_list;

    std::thread thread_updater;
    std::mutex mtx_session_list;

    int32_t max_peers;

    void cyclic_session_updater();

   public:
    SessionController(int32_t max_peers,
                      std::vector<Identity> const& identity_list);
    ~SessionController();
    void open_session(std::unique_ptr<InSocketAPI> client);
    void start_session_updating();
    void stop_session_updating();
    void close_sessions();
};
}

#endif