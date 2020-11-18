#pragma once
#include <arpa/inet.h>

#include <iostream>
#include <vector>

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

class Logger {
 private:
 public:
  static std::string time_format;
  static bool colors;

  Logger(){};
  virtual ~Logger();

  static void print_header(const std::string &address);
  static void log(const std::string &label, const std::string &buffer,
                  enum colors_e log_color = WHITE);
  static void log(const std::string &label, const std::vector<char> buffer,
                  enum colors_e log_color = WHITE);
  static void log(const struct sockaddr_in *address, const std::string &buffer,
                  enum colors_e log_color = WHITE);
  static void log(const struct sockaddr_in *address,
                  const std::vector<char> buffer,
                  enum colors_e log_color = WHITE);
};
