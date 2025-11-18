#pragma once

// IWYU pragma: begin_exports
#include "bit_count.h"
#include "integer.h"
#include "logical.h"
#include "rotation.h"
// IWYU pragma: end_exports

#include "rvi_instruction_registry.h"

#include <memory>

namespace rvi::rv32zbb {

inline void add_instructions(InstructionRegistry& registry) {
    // OP
    registry.add_instruction(std::make_unique<Andn>());
    registry.add_instruction(std::make_unique<Orn>());
    registry.add_instruction(std::make_unique<Xnor>());
    registry.add_instruction(std::make_unique<Max>());
    registry.add_instruction(std::make_unique<Maxu>());
    registry.add_instruction(std::make_unique<Min>());
    registry.add_instruction(std::make_unique<Minu>());
    registry.add_instruction(std::make_unique<Rol>());
    registry.add_instruction(std::make_unique<Ror>());
    // OP-IMM
    registry.add_instruction(std::make_unique<Clz>());
    registry.add_instruction(std::make_unique<Ctz>());
    registry.add_instruction(std::make_unique<Cpop>());
    registry.add_instruction(std::make_unique<Sextb>());
    registry.add_instruction(std::make_unique<Sexth>());
    registry.add_instruction(std::make_unique<Zext>());
    registry.add_instruction(std::make_unique<Rori>());
    registry.add_instruction(std::make_unique<Rev8>());
    registry.add_instruction(std::make_unique<Orcb>());
}

} // namespace rvi::rv32zbb

