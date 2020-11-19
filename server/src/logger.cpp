#include "logger.hpp"

#include <ctime>
#include <ostream>
std::string Logger::time_format = "%F %T";
bool Logger::colors = true;

std::string ip_to_str(const struct sockaddr_in *address) {
  std::string product;
  product = inet_ntoa(address->sin_addr);
  return product;
}

void select_color(enum colors_e color) {
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

void Logger::print_header(const std::string &label) {
  time_t rawtime;
  struct tm *timeinfo;
  char time_buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(time_buffer, sizeof(time_buffer), Logger::time_format.c_str(),
           timeinfo);

  std::string formatted_time(time_buffer);
  if (Logger::colors) select_color(CYAN);
  std::cout << "[" << formatted_time << "]";
  if (Logger::colors) select_color(BLUE);
  std::cout << "[" << label << "] ";
  if (Logger::colors) select_color(RESET);
}

void Logger::log(const struct sockaddr_in *address, const std::string &buffer,
                 enum colors_e log_color) {
  Logger::log(ip_to_str(address), buffer, log_color);
}

void Logger::log(const struct sockaddr_in *address,
                 const std::vector<char> buffer, enum colors_e log_color) {
  Logger::log(ip_to_str(address), buffer, log_color);
}

void Logger::log(const std::string &label, const std::string &buffer,
                 enum colors_e log_color) {
  Logger::print_header(label);
  if (Logger::colors) {
    select_color(log_color);
  }

  std::cout << buffer << std::endl;
  if (Logger::colors) select_color(RESET);
}

void Logger::log(const std::string &label, const std::vector<char> buffer,
                 enum colors_e log_color) {
  Logger::print_header(label);
  if (Logger::colors) {
    select_color(log_color);
  }

  for (auto &symbol : buffer) {
    if (isprint(symbol)) {
      std::cout << symbol;
      continue;
    }
    std::cout << "[" << static_cast<int>(symbol) << "]";
  }

  std::cout << std::endl; 
  if (Logger::colors) select_color(RESET);
}