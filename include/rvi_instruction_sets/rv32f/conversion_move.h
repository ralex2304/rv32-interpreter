#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

#include "config.h"

#include <bit>
#include <cmath>
#include <cstdint>

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

    enum FclassFields {
        NEG_INFINITY  = 1 << 0,
        NEG_NORMAL    = 1 << 1,
        NEG_SUBNORMAL = 1 << 2,
        NEG_ZERO      = 1 << 3,
        POS_ZERO      = 1 << 4,
        POS_SUBNORMAL = 1 << 5,
        POS_NORMAL    = 1 << 6,
        POS_INFINITY  = 1 << 7,
        SIGNALING_NAN = 1 << 8,
        QUIET_NAN     = 1 << 9,
    };

    static UnsignValue evaluate(FloatValue rs1) {
        switch (std::fpclassify(rs1)) {
            case FP_INFINITE:
                return (rs1 < 0) ? NEG_INFINITY : POS_INFINITY;

            case FP_NAN:
                if (std::numeric_limits<float>::has_signaling_NaN &&
                        std::bit_cast<uint32_t>(rs1) ==
                        std::bit_cast<uint32_t>(std::numeric_limits<float>::signaling_NaN()))
                    return SIGNALING_NAN;
                else
                    return QUIET_NAN;

            case FP_NORMAL:
                return (rs1 < 0) ? NEG_NORMAL : POS_NORMAL;

            case FP_SUBNORMAL:
                return (rs1 < 0) ? NEG_SUBNORMAL : POS_SUBNORMAL;

            case FP_ZERO:
                return (std::bit_cast<uint32_t>(rs1) >> 31) ? NEG_ZERO : POS_ZERO;

            default:
                throw Instruction::execution_error(std::format("fclass.s: unable to detect "
                            "{} = 0b{:b} float class", rs1, std::bit_cast<UnsignValue>(rs1)));
        }
    }
};
using Fclass = FloatMoveToReg<OperFclass>;

} // namespace rvi::rv32f

