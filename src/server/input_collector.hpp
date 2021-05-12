#ifndef INPUT_COLLECTOR_HPP
#define INPUT_COLLECTOR_HPP

#include <vector>

#include "src/network-hal/sockets_api.hpp"

using namespace NetworkHal;
namespace Server {
constexpr char escape_char = '\\';
constexpr char escape_symbol = 27;

template <size_t buffer_size>
class InputConstructor {
   private:
    constexpr static int32_t max_input_size = buffer_size * 2;
    std::unique_ptr<InSocketAPI> const& in_socket;
    std::vector<char> input;
    std::array<char, buffer_size> raw_rx_buff;
    std::atomic_flag& termination_flag;

    bool should_exit();
    void clear_rx_buffer();
    void store_and_clear_buffer(int64_t msg_length);
    void escape_charaters(int64_t last_msg_length);

   public:
    InputConstructor(std::unique_ptr<InSocketAPI> const& in_socket,
                     std::atomic_flag& termination_flag);
    ~InputConstructor() = default;
    void flush_input();
    void pool_for_respond();
    std::string get_response_str();
    std::vector<char> get_response_vec();
};

template class InputConstructor<recv_packet_size>;
}
#endif