#pragma once

#include "rvi_logger.h"

#include <cxxopts.hpp>
#include <string>
#include <vector>

namespace rvi {

class ArgParser {
    public:
        ArgParser(const int argc, const char* argv[]);

        bool is_help() {
            return is_help_;
        }

        Logger::Verbosity get_stderr_verbosity() {
            return stderr_verbosity_;
        }

        const std::vector<std::pair<const std::string&, Logger::Verbosity>>& get_log_files() {
            return log_files_;
        }

        const std::vector<std::string>& get_target_arguments();

    private:
        cxxopts::ParseResult result_;

        std::vector<std::pair<const std::string&, Logger::Verbosity>> log_files_;

        bool is_help_;

        Logger::Verbosity stderr_verbosity_ = Logger::DEFAULT_STDERR_VERBOSITY;

        Logger::Verbosity parse_verbosity_name_(std::string name);
};

} // namespace rvi

