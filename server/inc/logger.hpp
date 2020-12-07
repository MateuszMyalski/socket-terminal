#pragma once
#include <arpa/inet.h>
#include <assert.h>

#include <ctime>
#include <iostream>
#include <ostream>
#include <vector>

#define TIME_FORMAT "%F %T"

inline bool color_logs = false;

namespace Logger {
enum colors_e {
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
  RESET,
};

inline void select_color(enum colors_e color) {
  if (!color_logs) return;
  
  switch (color) {
    case BLACK:
      std::cout << "\u001b[30m";
      break;
    case RED:
      std::cout << "\u001b[31m";
      break;
    case GREEN:
      std::cout << "\u001b[32m";
      break;
    case YELLOW:
      std::cout << "\u001b[33m";
      break;
    case BLUE:
      std::cout << "\u001b[34m";
      break;
    case MAGENTA:
      std::cout << "\u001b[35m";
      break;
    case CYAN:
      std::cout << "\u001b[36m";
      break;
    case WHITE:
      std::cout << "\u001b[37m";
      break;
    case RESET:
      std::cout << "\u001b[0m";
      break;
    default:
      break;
  }
}

inline std::string ip_to_str(const struct sockaddr_in *address) {
  assert(address);
  std::string product;
  product = inet_ntoa(address->sin_addr);
  return product;
}

inline void print_header(const std::string &address) {
  time_t rawtime;
  struct tm *timeinfo;
  char time_buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(time_buffer, sizeof(time_buffer), TIME_FORMAT, timeinfo);

  std::string formatted_time(time_buffer);

  select_color(CYAN);

  std::cout << "[" << formatted_time << "]";

  select_color(BLUE);

  std::cout << "[" << address << "] ";

  select_color(RESET);
}

inline void log(const std::string &label, const std::string &buffer,
                enum colors_e log_color) {
  print_header(label);

  select_color(log_color);

  std::cout << buffer << std::endl;

  select_color(RESET);
}

inline void log(const std::string &label, const std::vector<char> buffer,
                enum colors_e log_color) {
  print_header(label);

  select_color(log_color);

  for (auto &symbol : buffer) {
    if (isprint(symbol)) {
      std::cout << symbol;
      continue;
    }
    std::cout << "[" << static_cast<int>(symbol) << "]";
  }

  std::cout << std::endl;

  select_color(RESET);
}

inline void log(const struct sockaddr_in *address, const std::string &buffer,
                enum colors_e log_color) {
  log(ip_to_str(address), buffer, log_color);
}

inline void log(const struct sockaddr_in *address,
                const std::vector<char> buffer, enum colors_e log_color) {
  log(ip_to_str(address), buffer, log_color);
}

}  // namespace Logger
