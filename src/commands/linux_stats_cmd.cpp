#include "linux_stats_cmd.hpp"

#include <sys/sysinfo.h>

#include <fstream>
#include <sstream>

#include "server/client_session.hpp"
#include "server/srv_def.hpp"
#include "utils/logger.hpp"

namespace Commands {

std::string generate_json_response(std::string_view command_name,
                                   std::string_view status,
                                   std::string_view data,
                                   std::string_view unit) {
    return "{"
           "\"command\":\"" +
           std::string(command_name) +
           "\","
           "\"unit\":\"" +
           std::string(unit) +
           "\","
           "\"status\":\"" +
           std::string(status) +
           "\","
           "\"text_logs\": [],"
           "\"data\": \"" +
           std::string(data) +
           "\""
           "}\r\n";
}

void CPUUsage::execute(void* ctx, const std::vector<std::string>& args) {
    auto ctx_client = static_cast<Server::ClientSession*>(ctx);
    std::stringstream msg_stream;

    std::string response;

    std::ifstream load_avg_file;
    load_avg_file.open("/proc/loadavg");

    std::string data_line;
    std::getline(load_avg_file, data_line, ' ');
    load_avg_file.close();

    response = generate_json_response("CPUUsage", "OK", data_line, "%");

    ctx_client->schedule_msg(response);

    ctx_client->send_scheduled();
}

std::string CPUUsage::get_man() {
    return "Returns JSON formatted CPU usage data.";
}

void RAMUsage::execute(void* ctx,
                       [[maybe_unused]] const std::vector<std::string>& args) {
    auto ctx_client = static_cast<Server::ClientSession*>(ctx);
    std::stringstream msg_stream;

    std::string response;

    struct sysinfo sys_info = {};

    int err = sysinfo(&sys_info);
    if (0 != err) {
        response = generate_json_response("RAMUsage", "Error", "", "");
    }

    float total_used_ram =
        (sys_info.totalram - sys_info.freeram) / (1024 * 1024);
    float used_ram_prnct =
        (total_used_ram / (sys_info.totalram / (1024 * 1024))) * 100;

    response = generate_json_response("RAMUsage", "OK",
                                      std::to_string(used_ram_prnct), "%");

    ctx_client->schedule_msg(response);

    ctx_client->send_scheduled();
}

std::string RAMUsage::get_man() {
    return "Returns JSON formatted RAM usage data.";
}

void TempZone::execute(void* ctx,
                       [[maybe_unused]] const std::vector<std::string>& args) {
    auto ctx_client = static_cast<Server::ClientSession*>(ctx);
    std::stringstream msg_stream;
    std::string response;
    std::ifstream temp_zone_file;
    temp_zone_file.open("/sys/class/thermal/thermal_zone" +
                        std::to_string(zone_number) + "/temp");

    std::string data_line;
    std::getline(temp_zone_file, data_line);
    temp_zone_file.close();
    float temp_c = atoi(data_line.c_str());

    response = generate_json_response("TempZone", "OK",
                                      std::to_string(temp_c / 1000), "*C");

    ctx_client->send_scheduled();
    ctx_client->schedule_msg(response);

    ctx_client->send_scheduled();
}

std::string TempZone::get_man() {
    return "Returns JSON formatted temperature usage data.";
}
}
