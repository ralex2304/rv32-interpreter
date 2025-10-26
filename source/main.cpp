#include "rvi.h"
#include "rvi_arg_parser.h"
#include "rvi_logger.h"

#include <cxxopts.hpp>

using namespace rvi;
using namespace std::literals;

int main(const int argc, const char* argv[]) {
    Logger::init();

    try {
        ArgParser arg_parser(argc, argv);

        if (arg_parser.is_help())
            return 0;

        Logger::set_stderr_verbosity(arg_parser.get_stderr_verbosity());
        Logger::add_log_files(arg_parser.get_log_files());

        auto& target_arguments = arg_parser.get_target_arguments();

        Rvi interpreter(target_arguments[0], target_arguments);

        interpreter.run();

        return interpreter.get_exit_code();
    } catch (const ArgParser::exception& e) {
        LOG_F(ERROR, "%s", ("Invalid arguments given: "s + e.what()).c_str());
        return -1;
    } catch (const Rvi::execution_error& e) {
        LOG_F(ERROR, "%s", ("Interpreter error: "s + e.what()).c_str());
        return -2;
    } catch (const Rvi::riscv_exception& e) {
        LOG_F(ERROR, "%s", ("RISC-V exception occured: "s + e.what()).c_str());
        return -3;
    }
}

