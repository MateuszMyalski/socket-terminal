#pragma once
#include <arpa/inet.h>
#include <assert.h>

#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

namespace Utils {
inline bool color_logs = true;

enum class colors {
    black,
    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white,
    reset
};

namespace {
inline void select_color(std::stringstream& stream, colors color) {
    if (!color_logs) return;

    switch (color) {
        case colors::black:
            stream << "\u001b[30m";
            break;
        case colors::red:
            stream << "\u001b[31m";
            break;
        case colors::green:
            stream << "\u001b[32m";
            break;
        case colors::yellow:
            stream << "\u001b[33m";
            break;
        case colors::blue:
            stream << "\u001b[34m";
            break;
        case colors::magenta:
            stream << "\u001b[35m";
            break;
        case colors::cyan:
            stream << "\u001b[36m";
            break;
        case colors::white:
            stream << "\u001b[37m";
            break;
        case colors::reset:
            stream << "\u001b[0m";
            break;
        default:
            break;
    }
}

inline std::string get_local_time() {
    constexpr char time_format[] = "%F %T";

    time_t rawtime;
    struct tm* timeinfo;
    char time_buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_buffer, sizeof(time_buffer), time_format, timeinfo);

    return time_buffer;
}

inline std::string add_log_info() {
    std::stringstream stream;
    select_color(stream, colors::cyan);
    stream << "[" << get_local_time() << "]";
    select_color(stream, colors::reset);

    return stream.str();
}
}

// inline std::string ip_to_str(const struct sockaddr_in *address) {
//     assert(address);
//     std::string product;
//     product = inet_ntoa(address->sin_addr);
//     return product;
// }

inline void log(std::stringstream& buffer, colors color) {
    std::stringstream stream;
    stream << add_log_info();
    select_color(stream, color);
    stream << " " << buffer.str();
    select_color(stream, colors::reset);
    stream << std::endl;

    std::cout << stream.str();
}

inline void log(const char* buffer, colors color) {
    std::stringstream tmp(buffer);
    log(tmp, color);
}
}