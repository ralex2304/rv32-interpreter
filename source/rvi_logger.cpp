#include "rvi_logger.h"

#include <algorithm>
#include <loguru.hpp>

using namespace rvi;

Logger::Verbosity Logger::minimum_level_ = Logger::FATAL;

void Logger::init() {
    loguru::g_preamble_date   = false;
    loguru::g_preamble_time   = false;
    loguru::g_preamble_uptime = false;
    loguru::g_preamble_thread = false;
    loguru::g_preamble_file   = false;

    minimum_level_ = FATAL;
}

void Logger::set_stderr_verbosity(Verbosity stderr_verbosity) {
    loguru::g_stderr_verbosity = stderr_verbosity;
    minimum_level_ = std::max(minimum_level_, stderr_verbosity);
}

void Logger::add_log_files(const std::vector<std::pair<const std::string&,
                                                       Verbosity>>& log_files) {
    for (auto file: log_files) {
        loguru::add_file(file.first.c_str(), loguru::FileMode::Truncate, file.second);
        minimum_level_ = std::max(minimum_level_, file.second);
    }
}

bool Logger::is_level_needed(Verbosity level) {
    return minimum_level_ >= level;
}

