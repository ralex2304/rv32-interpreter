#pragma once

#include "rvi_instructions.h"
#include "rvi_opcodes.h"

#include <type_traits>

namespace rvi {

template <typename Oper>
struct LoadInstruction: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) override {
        Address base   = RegValGetter<Operands::RS1  >::get_val(*state, operands);
        Address offset = ImmValGetter<Operands::IMM_I>::get_val(*state, operands);

        typename Oper::ReadT value = state->mem.get<typename Oper::ReadT>(base + offset);

        RegisterNum rd = operands.get<RegisterNum, Operands::RD>();

        if constexpr (std::is_signed_v<typename Oper::ReadT>) {
            SignValue sign_value = static_cast<SignValue>(value);

            state->regs.set(rd, static_cast<UnsignValue>(sign_value));
        } else {
            state->regs.set(rd, static_cast<UnsignValue>(value));
        }

        state->pc.increment();
        return ExecStatus::SUCCESS;
    }
};

namespace rv32i {

struct OperLb {
    static constexpr const char* name = "lb";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::LOAD, 0b000, 0};
    using ReadT = int8_t;
};
using Lb = LoadInstruction<OperLb>;

struct OperLh {
    static constexpr const char* name = "lh";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::LOAD, 0b001, 0};
    using ReadT = int16_t;
};
using Lh = LoadInstruction<OperLh>;

struct OperLw {
    static constexpr const char* name = "lw";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::LOAD, 0b010, 0};
    using ReadT = int32_t;
};
using Lw = LoadInstruction<OperLw>;

struct OperLbu {
    static constexpr const char* name = "lbu";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::LOAD, 0b100, 0};
    using ReadT = uint8_t;
};
using Lbu = LoadInstruction<OperLbu>;

struct OperLhu {
    static constexpr const char* name = "lhu";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::LOAD, 0b101, 0};
    using ReadT = uint16_t;
};
using Lhu = LoadInstruction<OperLhu>;

} // namespace rv32i

} // namespace rvi

