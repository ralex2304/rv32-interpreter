#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

namespace rvi::rv32f {

template <class Oper>
struct FloatCompare: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        RegisterNum rd = operands.get<RegisterNum, Operands::RD>();

        state->regs.set(rd, Oper::evaluate(
                                FloatRegValGetter<Operands::RS1>::get_val(*state, operands),
                                FloatRegValGetter<Operands::RS2>::get_val(*state, operands)));

        state->pc.increment();
        return ExecStatus::SUCCESS;
    }
};

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif

struct OperFeqs {
    static constexpr const char* name = "feq.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP,
                                                                 0b010, 0b10100'00);

    static UnsignValue evaluate(FloatValue rs1, FloatValue rs2) {
        return (rs1 == rs2) ? 1 : 0;
    }
};
using Feqs = FloatCompare<OperFeqs>;

struct OperFlts {
    static constexpr const char* name = "flt.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP,
                                                                 0b001, 0b10100'00);

    static UnsignValue evaluate(FloatValue rs1, FloatValue rs2) {
        return (rs1 < rs2) ? 1 : 0;
    }
};
using Flts = FloatCompare<OperFlts>;

struct OperFles {
    static constexpr const char* name = "fle.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP,
                                                                 0b000, 0b10100'00);

    static UnsignValue evaluate(FloatValue rs1, FloatValue rs2) {
        return (rs1 <= rs2) ? 1 : 0;
    }
};
using Fles = FloatCompare<OperFles>;

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

} // namespace rvi::rv32f

