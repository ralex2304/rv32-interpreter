#pragma once

// IWYU pragma: begin_exports
#include "arithmetics.h"
#include "conversion_move.h"
#include "compare.h"
#include "fused_multiply_add.h"
#include "load_store.h"
// IWYU pragma: end_exports

#include "rvi_instruction_registry.h"

#include <memory>

namespace rvi::rv32f {

inline void add_instructions(InstructionRegistry& registry) {
    // LOAD-FP
    registry.add_instruction(std::make_unique<Flw>());
    // STORE-FP
    registry.add_instruction(std::make_unique<Fsw>());
    // OP-FP
    registry.add_instruction(std::make_unique<Fadd>());
    registry.add_instruction(std::make_unique<Fsub>());
    registry.add_instruction(std::make_unique<Fmul>());
    registry.add_instruction(std::make_unique<Fdiv>());
    registry.add_instruction(std::make_unique<Fsqrt>());
    registry.add_instruction(std::make_unique<Fsgnj>());
    registry.add_instruction(std::make_unique<Fsgnjn>());
    registry.add_instruction(std::make_unique<Fsgnjx>());
    registry.add_instruction(std::make_unique<Fmin>());
    registry.add_instruction(std::make_unique<Fmax>());
    registry.add_instruction(std::make_unique<Fcvtws>());
    registry.add_instruction(std::make_unique<Fcvtwus>());
    registry.add_instruction(std::make_unique<Fmvxw>());
    registry.add_instruction(std::make_unique<Fcvtsw>());
    registry.add_instruction(std::make_unique<Fcvtswu>());
    registry.add_instruction(std::make_unique<Fmvwx>());
    registry.add_instruction(std::make_unique<Feqs>());
    registry.add_instruction(std::make_unique<Flts>());
    registry.add_instruction(std::make_unique<Fles>());
    registry.add_instruction(std::make_unique<Fclass>());
    // FMA
    registry.add_instruction(std::make_unique<Fmadd>());
    registry.add_instruction(std::make_unique<Fmsub>());
    registry.add_instruction(std::make_unique<Fnmsub>());
    registry.add_instruction(std::make_unique<Fnmadd>());
}

} // namespace rvi::rv32f

