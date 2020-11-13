#pragma once
#include <iostream>

class SocketTerminalServer {
 private:
  std::string server_ip;
  unsigned short server_port;
  unsigned short max_connections;
  bool use_IPv4 = true;

 public:
  SocketTerminalServer(std::string server_ip, unsigned short server_port,
                       unsigned short max_connections);
  ~SocketTerminalServer();
};