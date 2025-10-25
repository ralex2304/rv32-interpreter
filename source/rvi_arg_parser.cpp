#include "rvi_arg_parser.h"

#include <algorithm>
#include <cctype>
#include <cxxopts.hpp>
#include <iostream>
#include "magic_enum_adapter.h"
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using namespace rvi;
using namespace std::literals;

static void add_options_(cxxopts::Options& options);

static void add_options_(cxxopts::Options& options) {
    std::string default_verb_name(magic_enum::enum_name(Logger::DEFAULT_STDERR_VERBOSITY));

    auto verb_names = magic_enum::enum_names<Logger::Verbosity>() | std::views::join_with(", "sv);

    options.add_options()
        ("v,verbosity", "stderr messages verbosity: "s +
                        std::string(verb_names.begin(), verb_names.end()),
            cxxopts::value<std::string>()->default_value(default_verb_name))

        ("l,log", "Create log file. You can specify verbosity as filename extension (default: DUMP). "
                  "E.g log.error", cxxopts::value<std::vector<std::string>>())

        ("h,help", "Print help")
    ;
}

ArgParser::ArgParser(const int argc, const char* argv[]) {
    cxxopts::Options options("rv32-interpreter", "RISC-V 32 bit interpreter");

    if (argc < 2 || argv[1][0] != '-') {
        options.allow_unrecognised_options();

        result_ = options.parse(argc, argv); //< shouldn't throw exception

        return;
    }

    add_options_(options);

    try {
        result_ = options.parse(argc, argv);
    } catch (const cxxopts::exceptions::parsing& e) {
        throw exception(e.what());
    }

    if (result_.count("help")) {
        std::cout << options.help() << std::endl;
        return;
    }

    if (result_.count("log")) {
        for (const auto& file: result_["log"].as<std::vector<std::string>>()) {
            size_t last_dot_pos = file.find_last_of('.');

            Logger::Verbosity verbosity = Logger::Verbosity::DUMP;

            if (last_dot_pos != std::string::npos) {
                std::string extension = file.substr(last_dot_pos + 1);
                std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);

                auto entry = magic_enum::enum_cast<Logger::Verbosity>(extension);
                if (entry.has_value())
                    verbosity = entry.value();
            }

            log_files_.push_back({file, verbosity});
        }
    }

    stderr_verbosity_ = parse_verbosity_name_(result_["verbosity"].as<std::string>());
}

const std::vector<std::string>& ArgParser::get_target_arguments() {

    const auto& args = result_.unmatched();

    if (args.size() == 0)
        throw exception("ELF file name is not specified");

    return args;
}

Logger::Verbosity ArgParser::parse_verbosity_name_(std::string name) {
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);

    auto entry = magic_enum::enum_cast<Logger::Verbosity>(name);

    if (!entry.has_value())
        throw exception("Invalid verbosity level name given: "s + name);

    return entry.value();
}

