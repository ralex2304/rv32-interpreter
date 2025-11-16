#pragma once

// IWYU pragma: begin_exports
#include "load_store.h"
// IWYU pragma: end_exports

#include "rvi_instruction_registry.h"

#include <memory>

namespace rvi::rv32f {

inline void add_instructions(InstructionRegistry& registry) {
    // LOAD FP
    registry.add_instruction(std::make_unique<Flw>());
    // STORE FP
    registry.add_instruction(std::make_unique<Fsw>());
}

} // namespace rvi::rv32f

