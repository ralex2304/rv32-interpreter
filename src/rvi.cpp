#include "rvi.h"

#include "rvi_datatypes.h"
#include "rvi_instruction_registry.h"
#include "rvi_instruction_sets/rv32i/rv32i.h"
#include "rvi_operands.h"

#include <format>
#include <loguru.hpp>
#include <stdexcept>
#include <string_view>
#include <vector>

using namespace rvi;

Rvi::Rvi(const std::filesystem::path elf_path, const std::vector<std::string_view>& args) {
    state_.load_elf(elf_path);
    state_.init_execution_environment(args);

    rv32i::add_instructions(registry_);
}

void Rvi::run() {
    for (;;) {
        const RawInstruction raw_instr = state_.mem.get<RawInstruction>(state_.pc.get());

        ExecStatus status = run_instruction_(raw_instr);

        if (status == ExecStatus::SUCCESS)
            continue;

        if (status == ExecStatus::EXIT)
            break;


        throw std::runtime_error(std::format("RISC-V exception occured (pc = 0x{:x}): {}",
                                             state_.pc.get(), magic_enum::enum_name(status)));
    }

}

ExecStatus Rvi::run_instruction_(const RawInstruction raw_instr) {
#ifdef LOGGING
    state_.regs.dump();

    LOG_SCOPE_F(INFO, "pc = %x instr = %.8x", state_.pc.get(), raw_instr);
#endif

    const Instruction* instr = registry_.get_instruction(raw_instr);

    if (instr == nullptr)
        return ExecStatus::ILLEGAL_INSTRUCTION;

    const Operands operands(raw_instr);

#ifdef LOGGING
    LOG_F(INFO, "%s", instr->get_name());
    operands.dump();
#endif

    return instr->execute(&state_, operands);
}

