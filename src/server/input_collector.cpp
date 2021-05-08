#include "src/server/input_collector.hpp"

#include "src/utils/logger.hpp"

using namespace NetworkHal;

namespace Server {
template <size_t buffer_size>
InputConstructor<buffer_size>::InputConstructor(
    std::unique_ptr<InSocketAPI> const& in_socket,
    std::atomic_flag& termination_flag)
    : in_socket(in_socket), termination_flag(termination_flag) {
    in_socket->set_socket_no_block();
    clear_rx_buffer();
    flush_input();
    input.reserve(max_input_size);
};

template <size_t buffer_size>
void InputConstructor<buffer_size>::store_and_clear_buffer(int64_t msg_length) {
    if (msg_length <= 0) {
        return;
    }

    auto it_base = raw_rx_buff.begin();
    input.insert(input.end(), it_base, it_base + msg_length);
    clear_rx_buffer();
};

template <size_t buffer_size>
void InputConstructor<buffer_size>::escape_charaters(int64_t last_msg_length) {
    if ((last_msg_length <= 0) || (last_msg_length > input.size())) {
        return;
    }

    auto start_it = input.end() - last_msg_length;
    for (; start_it != input.end(); start_it++) {
        if (*start_it != escape_char) {
            continue;
        }
        if ((*std::next(start_it) == '\n') || (*std::next(start_it) == '\r')) {
            *start_it = escape_symbol;
        } else {
            start_it = input.erase(start_it);
        }
    }
};

template <size_t buffer_size>
bool InputConstructor<buffer_size>::should_exit() {
    // User can use win or unix end of line style
    bool is_end_char_escaped = (*(input.end() - 3) == escape_symbol) ||
                               (*(input.end() - 2) == escape_symbol);
    bool end_char_occured = (input.back() == '\n') && !is_end_char_escaped;

    bool keep_session = !termination_flag.test_and_set();
    if (keep_session) {
        termination_flag.clear();
    }

    return keep_session || end_char_occured;
};

template <size_t buffer_size>
void inline InputConstructor<buffer_size>::clear_rx_buffer() {
    raw_rx_buff.fill('\0');
};

template <size_t buffer_size>
void inline InputConstructor<buffer_size>::flush_input() {
    input.clear();
};

template <size_t buffer_size>
void InputConstructor<buffer_size>::pool_for_respond() {
    flush_input();
    clear_rx_buffer();
    while (!should_exit()) {
        auto msg_lenght = in_socket->recv_buffer(raw_rx_buff);
        store_and_clear_buffer(msg_lenght);
        escape_charaters(msg_lenght);
    }
};

template <size_t buffer_size>
std::vector<char> InputConstructor<buffer_size>::get_response_vec() {
    return input;
};

template <size_t buffer_size>
std::string InputConstructor<buffer_size>::get_response_str() {
    std::string response;
    response.reserve(input.size());

    for (auto it = input.begin(); it != input.end(); it++) {
        if (isprint(*it)) {
            response.push_back(*it);
        }
    }
    return response;
};

template class InputConstructor<1024>;
}