#include "rvi.h"
#include "rvi_arg_parser.h"
#include "rvi_logger.h"

#include <cxxopts.hpp>
using namespace rvi;

int main(const int argc, const char* argv[]) {
    ArgParser arg_parser(argc, argv);

    if (arg_parser.is_help())
        return 0;

    Logger logger(arg_parser.get_stderr_verbosity(), arg_parser.get_log_files());

    auto& target_arguments = arg_parser.get_target_arguments();

    Rvi interpreter(target_arguments[0], target_arguments);

    interpreter.run();

    return interpreter.get_exit_code();
}

