#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

#include "config.h"
#include <bit>

namespace rvi::rv32f {

template <class Oper>
struct FloatMoveToReg: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        RegisterNum rd = operands.get<RegisterNum, Operands::RD>();

        state->regs.set(rd, Oper::evaluate(FloatRegValGetter<Operands::RS1>::get_val(*state,
                                                                                     operands)));

        state->pc.increment();
        return ExecStatus::SUCCESS;
    }
};

template <class Oper>
struct FloatMoveToFreg: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        RegisterNum rd = operands.get<RegisterNum, Operands::RD>();

        state->fregs[rd] = Oper::evaluate(RegValGetter<Operands::RS1>::get_val(*state, operands));

        state->pc.increment();
        return ExecStatus::SUCCESS;
    }
};

struct OperFcvtws {
    static constexpr const char* name = "fcvt.w.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7_width(PlainOpcodes::OP_FP,
                                                     FCVTWS_ROUND_MODE, 0b00000, 0b11000'00);

    static UnsignValue evaluate(FloatValue rs1) {
        return std::bit_cast<UnsignValue>(static_cast<SignValue>(rs1));
    }
};
using Fcvtws = FloatMoveToReg<OperFcvtws>;

struct OperFcvtwus {
    static constexpr const char* name = "fcvt.wu.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7_width(PlainOpcodes::OP_FP,
                                                     FCVTWS_ROUND_MODE, 0b00001, 0b11000'00);

    static UnsignValue evaluate(FloatValue rs1) {
        return static_cast<UnsignValue>(rs1);
    }
};
using Fcvtwus = FloatMoveToReg<OperFcvtwus>;

struct OperFmvxw {
    static constexpr const char* name = "fmv.x.w";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7_width(PlainOpcodes::OP_FP,
                                                                 0b000, 0b00000, 0b11100'00);

    static UnsignValue evaluate(FloatValue rs1) {
        return std::bit_cast<UnsignValue>(rs1);
    }
};
using Fmvxw = FloatMoveToReg<OperFmvxw>;

struct OperFcvtsw {
    static constexpr const char* name = "fcvt.s.w";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7_width(PlainOpcodes::OP_FP,
                                                            ROUND_MODE, 0b00000, 0b11010'00);

    static FloatValue evaluate(UnsignValue rs1) {
        return static_cast<FloatValue>(std::bit_cast<SignValue>(rs1));
    }
};
using Fcvtsw = FloatMoveToFreg<OperFcvtsw>;

struct OperFcvtswu {
    static constexpr const char* name = "fcvt.s.wu";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7_width(PlainOpcodes::OP_FP,
                                                            ROUND_MODE, 0b00001, 0b11010'00);

    static FloatValue evaluate(UnsignValue rs1) {
        return static_cast<FloatValue>(rs1);
    }
};
using Fcvtswu = FloatMoveToFreg<OperFcvtswu>;

struct OperFmvwx {
    static constexpr const char* name = "fmv.w.x";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7_width(PlainOpcodes::OP_FP,
                                                                 0b000, 0b00000, 0b11110'00);

    static FloatValue evaluate(UnsignValue rs1) {
        return std::bit_cast<FloatValue>(rs1);
    }
};
using Fmvwx = FloatMoveToFreg<OperFmvwx>;

struct OperFclass {
    static constexpr const char* name = "fclass.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7_width(PlainOpcodes::OP_FP,
                                                                 0b001, 0b00000, 0b11100'00);

    static UnsignValue evaluate(FloatValue rs1) {
        // TODO: fclass.s
        throw Instruction::execution_error(std::format("fclass.s: unable to detect "
                        "{} = 0b{:b} float class", rs1, std::bit_cast<UnsignValue>(rs1)));
    }
};
using Fclass = FloatMoveToReg<OperFclass>;

} // namespace rvi::rv32f

