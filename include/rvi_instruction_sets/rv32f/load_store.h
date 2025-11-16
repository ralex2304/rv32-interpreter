#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include <bit>

namespace rvi::rv32f {

struct Flw: Instruction {
    constexpr const char* get_name() const noexcept override {
        return "flw";
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return OpcodeFunct_3(PlainOpcodes::LOAD_FP, 0b010);
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        Address base   = RegValGetter<Operands::RS1  >::get_val(*state, operands);
        Address offset = ImmValGetter<Operands::IMM_I>::get_val(*state, operands);

        RegisterNum rd = operands.get<RegisterNum, Operands::RD>();

        state->fregs[rd] = std::bit_cast<FloatValue>(state->mem.get<UnsignValue>(base + offset));

        state->pc.increment();
        return ExecStatus::SUCCESS;
    }
};


struct Fsw: Instruction {
    constexpr const char* get_name() const noexcept override {
        return "fsw";
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return OpcodeFunct_3(PlainOpcodes::STORE_FP, 0b010);
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        Address base = RegValGetter<Operands::RS1  >::get_val(*state, operands);
        Immediate offset = ImmValGetter<Operands::IMM_S>::get_val(*state, operands);

        FloatValue value = FloatRegValGetter<Operands::RS2>::get_val(*state, operands);

        state->mem.set(base + offset, std::bit_cast<UnsignValue>(value));

        state->pc.increment();
        return ExecStatus::SUCCESS;
    };
};

} // namespace rvi::rv32f

