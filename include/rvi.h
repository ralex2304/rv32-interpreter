#pragma once

#include "rvi_datatypes.h"
#include "rvi_instruction_registry.h"

#include <bit>
#include <filesystem>
#include <vector>

namespace rvi {

class Rvi {
    public:
        Rvi(const std::filesystem::path elf_path, const std::vector<std::string>& args);

        void run();

        int get_exit_code() const {
            int ret_code = std::bit_cast<int>(state_.regs.get(10));
            LOG_F(INFO, "Return code %d", ret_code);
            return ret_code;
        }

    private:
        InstructionRegistry registry_;
        RviState state_;

        ExecStatus run_instruction_(const RawInstruction raw_instr);
};

} // namespace rvi

