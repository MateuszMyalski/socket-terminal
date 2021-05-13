#ifndef SERVER_SRV_DEF_HPP
#define SERVER_SRV_DEF_HPP

#include <stdint.h>

#include <chrono>

using namespace std::chrono;
namespace Server {
constexpr int64_t recv_packet_size = 1024;

constexpr int invalid_auth_max_tries = 3;

constexpr duration<int, std::milli> session_refresh_delay(500);
constexpr duration<int, std::milli> session_timeout_t(5000);  //(1 << 15);

}
#endif