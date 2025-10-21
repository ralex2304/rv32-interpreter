#pragma once

// IWYU pragma: begin_exports
#include "arithmetics.h"
#include "jals.h"
#include "branches.h"
#include "loads.h"
#include "stores.h"
#include "misc_mems.h"
#include "systems.h"
// IWYU pragma: end_exports

/* Unimplemented Instructions:
 *  - FENCE
 *  - TODO: EBREAK
 */

#include "rvi_instruction_registry.h"

#include <memory>

namespace rvi {

namespace rv32i {

inline void add_instructions(InstructionRegistry& registry) {
    /// LUI
    registry.add_instruction(std::make_unique<Lui>());
    /// AUIPC
    registry.add_instruction(std::make_unique<Auipc>());
    /// OP_IMM
    registry.add_instruction(std::make_unique<Addi>());
    registry.add_instruction(std::make_unique<Slti>());
    registry.add_instruction(std::make_unique<Sltiu>());
    registry.add_instruction(std::make_unique<Xori>());
    registry.add_instruction(std::make_unique<Ori>());
    registry.add_instruction(std::make_unique<Andi>());
    registry.add_instruction(std::make_unique<Slli>());
    registry.add_instruction(std::make_unique<Srli>());
    registry.add_instruction(std::make_unique<Srai>());
    registry.add_instruction(std::make_unique<Add>());
    registry.add_instruction(std::make_unique<Sub>());
    registry.add_instruction(std::make_unique<Sll>());
    registry.add_instruction(std::make_unique<Slt>());
    registry.add_instruction(std::make_unique<Sltu>());
    registry.add_instruction(std::make_unique<Xor>());
    registry.add_instruction(std::make_unique<Srl>());
    registry.add_instruction(std::make_unique<Sra>());
    registry.add_instruction(std::make_unique<Or>());
    registry.add_instruction(std::make_unique<And>());
    /// JAL
    registry.add_instruction(std::make_unique<Jal>());
    /// JALR
    registry.add_instruction(std::make_unique<Jalr>());
    /// BRANCH
    registry.add_instruction(std::make_unique<Beq>());
    registry.add_instruction(std::make_unique<Bne>());
    registry.add_instruction(std::make_unique<Blt>());
    registry.add_instruction(std::make_unique<Bge>());
    registry.add_instruction(std::make_unique<Bltu>());
    registry.add_instruction(std::make_unique<Bgeu>());
    /// LOAD
    registry.add_instruction(std::make_unique<Lb>());
    registry.add_instruction(std::make_unique<Lh>());
    registry.add_instruction(std::make_unique<Lw>());
    registry.add_instruction(std::make_unique<Lbu>());
    registry.add_instruction(std::make_unique<Lhu>());
    /// STORE
    registry.add_instruction(std::make_unique<Sb>());
    registry.add_instruction(std::make_unique<Sh>());
    registry.add_instruction(std::make_unique<Sw>());
    /// MISC_MEM
    registry.add_instruction(std::make_unique<Fence>());
    /// SYSTEM
    registry.add_instruction(std::make_unique<Ecall>());
    registry.add_instruction(std::make_unique<Ebreak>());
}

} // namespace rv32i

} // namespace rvi

