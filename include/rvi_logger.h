#pragma once

#include <loguru.hpp> // IWYU pragma: export
#include <string>
#include <vector>

namespace rvi {

class Logger {
public:
    enum Verbosity {
        FATAL   = -3,
        ERROR   = -2,
        WARNING = -1,
        INFO    =  0,
        DUMP    =  1,
    };

    static constexpr Verbosity DEFAULT_STDERR_VERBOSITY = ERROR;

    static void init();

    static void set_stderr_verbosity(Verbosity stderr_verbosity);

    static void add_log_files(const std::vector<std::pair<const std::string&,
                                                          Verbosity>>& log_files);

    static bool is_level_needed(Verbosity level);

private:
    static Verbosity minimum_level_;
};

} // namespace rvi

namespace loguru {

constexpr int Verbosity_DUMP = rvi::Logger::DUMP;

} // namespace loguru

