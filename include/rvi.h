#pragma once

#include "rvi_datatypes.h"
#include "rvi_instruction_registry.h"

#include <filesystem>
#include <string_view>
#include <vector>

namespace rvi {

class Rvi {
    public:
        Rvi(const std::filesystem::path elf_path, const std::vector<std::string_view>& args);

        void run();

        int get_exit_code() const {
            return static_cast<int>(state_.regs.get(10));
        }

    private:
        InstructionRegistry registry_;
        RviState state_;

        ExecStatus run_instruction_(const RawInstruction raw_instr);
};

} // namespace rvi

