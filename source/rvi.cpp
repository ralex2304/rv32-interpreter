#include "rvi.h"

#include "rvi_datatypes.h"
#include "rvi_instruction_registry.h"
#include "rvi_instruction_sets/rv32i/rv32i.h"
#include "rvi_instructions.h"
#include "rvi_logger.h"
#include "rvi_operands.h"

#include <format>
#include <vector>

using namespace rvi;
using namespace std::literals;

Rvi::Rvi(const std::filesystem::path elf_path, const std::vector<std::string>& args) {
    state_.load_elf(elf_path);
    state_.init_execution_environment(args);

    try {
        LOG_F(INFO, "Registry: rv32i extension");
        rv32i::add_instructions(registry_);
    } catch (const InstructionRegistry::exception& e) {
        throw execution_error("Instruction registry error: "s + e.what());
    }
}

void Rvi::run() {
    for (;;) {
        const RawInstruction raw_instr = state_.mem.get<RawInstruction>(state_.pc.get());

        ExecStatus status = run_instruction_(raw_instr);

        if (status == ExecStatus::SUCCESS)
            continue;

        if (status == ExecStatus::EXIT)
            break;

        throw riscv_exception(std::format("pc = 0x{:x} - {}", state_.pc.get(),
                                          magic_enum::enum_name(status)));
    }

}

ExecStatus Rvi::run_instruction_(const RawInstruction raw_instr) {
    state_.regs.dump();

    LOG_F(INFO, "pc = %x instr = %.8x", state_.pc.get(), raw_instr);

    const Instruction* instr = registry_.get_instruction(raw_instr);

    if (instr == nullptr)
        return ExecStatus::ILLEGAL_INSTRUCTION;

    const Operands operands(raw_instr);

    LOG_F(INFO, "%s", instr->get_name());
    operands.dump();

    try {
        return instr->execute(&state_, operands);
    } catch (const Instruction::execution_error& e) {
        throw execution_error("Instruction execution error: "s + e.what());
    }
}

