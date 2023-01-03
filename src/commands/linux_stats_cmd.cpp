#include "linux_stats_cmd.hpp"

#include <sstream>

#include "server/client_session.hpp"
#include "server/srv_def.hpp"
#include "utils/logger.hpp"

namespace Commands {
void CPUUsage::execute(void* ctx, const std::vector<std::string>& args) {}

std::string CPUUsage::get_man() {
    return "Returns JSON formatted CPU usage data.";
}

void RAMUsage::execute(void* ctx,
                       [[maybe_unused]] const std::vector<std::string>& args) {}

std::string RAMUsage::get_man() {
    return "Returns JSON formatted RAM usage data.";
}

}
