#include "rvi_datatypes.h"
#include "rvi_instruction_registry.h"
#include "rvi_instruction_sets/rv32i/rv32i.h"
#include "rvi_instructions.h"
#include "rvi_operands.h"
#include "rvi_state.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <vector>

using namespace rvi;

int main(const int argc, const char* argv[]) {
    if (argc < 2)
        throw std::runtime_error("ELF file must be given as first argument");

    std::vector<std::string_view> arguments(static_cast<size_t>(argc - 2));
    for (size_t i = 0; i < static_cast<size_t>(argc - 2); i++)
        arguments.push_back(argv[static_cast<size_t>(argc - 2) + i]);

    RviState state;

    state.load_elf(argv[1]);
    state.init_execution_environment(arguments);

    InstructionRegistry registry;
    rv32i::add_instructions(registry);

    ExecStatus status = ExecStatus::SUCCESS;
    while (status == ExecStatus::SUCCESS) {
        const RawInstruction raw_instr = state.mem.get<RawInstruction>(state.pc.get());

        const Instruction* instr = registry.get_instruction(raw_instr);

        if (instr == nullptr)
            return static_cast<int>(ExecStatus::ILLEGAL_INSTRUCTION);

        const Operands operands(raw_instr);

        std::cout << std::hex << state.pc.get() << ": " << instr->get_name() << std::endl;

        status = instr->execute(&state, operands);
    }

    return static_cast<int>(status);
}

