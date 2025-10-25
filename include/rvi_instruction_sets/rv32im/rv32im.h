#pragma once

// IWYU pragma: begin_exports
#include "mul.h"
#include "div.h"
// IWYU pragma: end_exports

#include "rvi_instruction_registry.h"

#include <memory>

namespace rvi {

namespace rv32im {

inline void add_instructions(InstructionRegistry& registry) {
    /// OP
    registry.add_instruction(std::make_unique<Mul>());
    registry.add_instruction(std::make_unique<Mulh>());
    registry.add_instruction(std::make_unique<Mulhsu>());
    registry.add_instruction(std::make_unique<Mulhu>());
    registry.add_instruction(std::make_unique<Div>());
    registry.add_instruction(std::make_unique<Divu>());
    registry.add_instruction(std::make_unique<Rem>());
    registry.add_instruction(std::make_unique<Remu>());
}

} // namespace rv32im

} // namespace rvi

