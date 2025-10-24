#pragma once

#include <loguru.hpp> // IWYU pragma: export
#include <string>
#include <vector>

namespace rvi {

struct Logger {
    enum Verbosity {
        FATAL   = -3,
        ERROR   = -2,
        WARNING = -1,
        INFO    =  0,
        DUMP    =  1,
    };

    static constexpr Verbosity DEFAULT_STDERR_VERBOSITY = ERROR;

    Logger() {
        loguru::g_preamble_date   = false;
        loguru::g_preamble_time   = false;
        loguru::g_preamble_uptime = false;
        loguru::g_preamble_thread = false;
        loguru::g_preamble_file   = false;
    }

    void set_stderr_verbosity(Verbosity stderr_verbosity) {
        loguru::g_stderr_verbosity = stderr_verbosity;
    }

    void add_log_files(const std::vector<std::pair<const std::string&, Verbosity>>& log_files) {
        for (auto file: log_files)
            loguru::add_file(file.first.c_str(), loguru::FileMode::Truncate, file.second);

    }
};

} // namespace rvi

namespace loguru {

constexpr int Verbosity_DUMP = rvi::Logger::DUMP;

} // namespace loguru

