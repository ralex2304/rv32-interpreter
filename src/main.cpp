#include "rvi.h"

#include <cstddef>
#include <stdexcept>
#include <string_view>
#include <vector>

using namespace rvi;

int main(const int argc, const char* argv[]) {
    loguru::g_preamble_date = false;
    loguru::g_preamble_time = false;
    loguru::g_preamble_uptime = false;
    loguru::g_preamble_thread = false;
    loguru::g_preamble_file = false;

    if (argc < 2)
        throw std::runtime_error("ELF file must be given as first argument");

    std::vector<std::string_view> arguments(static_cast<size_t>(argc - 2));
    for (size_t i = 0; i < static_cast<size_t>(argc - 2); i++)
        arguments.push_back(argv[static_cast<size_t>(argc - 2) + i]);

    Rvi interpreter(argv[1], arguments);

    interpreter.run();

    return interpreter.get_exit_code();
}

