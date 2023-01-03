#ifndef LINUX_STATS_CMD_HPP
#define LINUX_STATS_CMD_HPP

#include "command.hpp"

namespace Commands {
class CPUUsage : public Command {
   public:
    void execute(void* ctx, const std::vector<std::string>& args);
    std::string get_man();
};

class RAMUsage : public Command {
   public:
    void execute(void* ctx, const std::vector<std::string>& args);
    std::string get_man();
};

class TempZone : public Command {
    int zone_number = 0;

   public:
    TempZone(int zone_no) : zone_number(zone_no){};
    void execute(void* ctx, const std::vector<std::string>& args);
    std::string get_man();
};

}
#endif