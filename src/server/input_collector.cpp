#include "input_collector.hpp"

#include <iterator>

#include "utils/logger.hpp"

using namespace NetworkHal;

namespace Server {
namespace {
template <typename T>
inline int32_t last_escape_pos(T data) {
    // User can use win or unix end of line style
    if (data.size() < 2) {
        return 0;
    }

    auto rev_it = data.rbegin();
    std::advance(rev_it, 2);

    if (*rev_it == escape_symbol) {
        return 3;
    }

    if (data.size() < 3) {
        return 0;
    }

    std::advance(rev_it, 3);
    if (*rev_it == escape_symbol) {
        return 2;
    }
    return 0;
}
}
template <size_t buffer_size>
InputConstructor<buffer_size>::InputConstructor(
    std::unique_ptr<InSocketAPI> const& in_socket,
    std::atomic_flag& termination_flag)
    : in_socket(in_socket), termination_flag(termination_flag) {
    in_socket->set_socket_no_block();
    clear_rx_buffer();
    flush_input();
    input.reserve(max_input_size);
}

template <size_t buffer_size>
void inline InputConstructor<buffer_size>::clear_rx_buffer() {
    raw_rx_buff.fill('\0');
}

template <size_t buffer_size>
void inline InputConstructor<buffer_size>::flush_input() {
    input.clear();
}

template <size_t buffer_size>
void InputConstructor<buffer_size>::store_and_clear_buffer(int64_t msg_length) {
    if (msg_length <= 0) {
        return;
    }

    auto it_base = raw_rx_buff.begin();
    input.insert(input.end(), it_base, it_base + msg_length);
    clear_rx_buffer();
}

template <size_t buffer_size>
bool InputConstructor<buffer_size>::is_eol_escaped() {
    int32_t escape_pos = last_escape_pos(input);
    if (0 == escape_pos) {
        return false;
    }
    Utils::warning("here");

    int32_t escape_cnt = 0;
    auto rev_it = input.rbegin() + escape_pos;
    for (; rev_it != input.rend(); rev_it++) {
        if (*rev_it == escape_char) {
            escape_cnt++;
        } else {
            break;
        }
    }

    return (escape_cnt % 2) != 0;
}

template <size_t buffer_size>
bool InputConstructor<buffer_size>::should_exit() {
    bool is_end_char_escaped = is_eol_escaped();
    bool end_char_occured = (input.back() == '\n') && !is_end_char_escaped;

    bool keep_session = !termination_flag.test_and_set();
    if (keep_session) {
        termination_flag.clear();
    }

    return keep_session || end_char_occured;
}

template <size_t buffer_size>
void InputConstructor<buffer_size>::pool_for_respond() {
    flush_input();
    clear_rx_buffer();
    while (!should_exit()) {
        auto msg_lenght = in_socket->recv_buffer(raw_rx_buff);
        store_and_clear_buffer(msg_lenght);
    }
}

template <size_t buffer_size>
std::vector<char> InputConstructor<buffer_size>::get_response_vec() {
    return input;
}

template <size_t buffer_size>
std::string InputConstructor<buffer_size>::get_response_str() {
    std::string response;
    response.reserve(input.size());

    for (auto it = input.begin(); it != input.end(); it++) {
        if (isprint(*it) && ((*it) != '\n') && ((*it) != '\r')) {
            response.push_back(*it);
        }
    }
    return response;
}
}