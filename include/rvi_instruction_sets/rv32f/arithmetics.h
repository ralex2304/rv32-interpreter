#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_logger.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

#include "config.h"

#include <cmath>

namespace rvi::rv32f {

template <class Oper, class... ValuesGetters>
struct FloatArithmInstruction: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        RegisterNum rd = operands.get<RegisterNum, Operands::RD>();

        state->fregs[rd] = Oper::evaluate(ValuesGetters::get_val(*state, operands)...);

        state->pc.increment();
        return ExecStatus::SUCCESS;
    }
};

struct OperFadd {
    static constexpr const char* name = "fadd.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP, ROUND_MODE,
                                                                 0b00000'00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2) {
        return rs1 + rs2;
    }
};
using Fadd = FloatArithmInstruction<OperFadd, FloatRegValGetter<Operands::RS1>,
                                              FloatRegValGetter<Operands::RS2>>;

struct OperFsub {
    static constexpr const char* name = "fsub.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP, ROUND_MODE,
                                                                 0b00001'00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2) {
        return rs1 - rs2;
    }
};
using Fsub = FloatArithmInstruction<OperFsub, FloatRegValGetter<Operands::RS1>,
                                              FloatRegValGetter<Operands::RS2>>;

struct OperFmul {
    static constexpr const char* name = "fmul.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP, ROUND_MODE,
                                                                 0b00010'00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2) {
        return rs1 * rs2;
    }
};
using Fmul = FloatArithmInstruction<OperFmul, FloatRegValGetter<Operands::RS1>,
                                              FloatRegValGetter<Operands::RS2>>;

struct OperFdiv {
    static constexpr const char* name = "fdiv.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP, ROUND_MODE,
                                                                 0b00011'00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2) {
        return rs1 / rs2;
    }
};
using Fdiv = FloatArithmInstruction<OperFdiv, FloatRegValGetter<Operands::RS1>,
                                              FloatRegValGetter<Operands::RS2>>;

struct OperFsqrt {
    static constexpr const char* name = "fsqrt.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_5_7(PlainOpcodes::OP_FP,
                                                            ROUND_MODE, 0b00000, 0b01011'00);

    static FloatValue evaluate(FloatValue rs1) {
        return std::sqrt(rs1);
    }
};
using Fsqrt = FloatArithmInstruction<OperFsqrt, FloatRegValGetter<Operands::RS1>>;

struct OperFsgnj {
    static constexpr const char* name = "fsgnj.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP, 0b000,
                                                                 0b00100'00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2) {
        return std::copysign(rs1, rs2);
    }
};
using Fsgnj = FloatArithmInstruction<OperFsgnj, FloatRegValGetter<Operands::RS1>,
                                                FloatRegValGetter<Operands::RS2>>;

struct OperFsgnjn {
    static constexpr const char* name = "fsgnjn.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP, 0b001,
                                                                 0b00100'00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2) {
        return -std::copysign(rs1, rs2);
    }
};
using Fsgnjn = FloatArithmInstruction<OperFsgnjn, FloatRegValGetter<Operands::RS1>,
                                                  FloatRegValGetter<Operands::RS2>>;

struct OperFsgnjx {
    static constexpr const char* name = "fsgnjx.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP, 0b010,
                                                                 0b00100'00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2) {
        bool sign1 = std::signbit(rs1);
        bool sign2 = std::signbit(rs2);
        return std::copysign(rs1, (sign1 ^ sign2) ? -1.0f : 1.0f);
    }
};
using Fsgnjx = FloatArithmInstruction<OperFsgnjx, FloatRegValGetter<Operands::RS1>,
                                                  FloatRegValGetter<Operands::RS2>>;

struct OperFmin {
    static constexpr const char* name = "fmin.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP, 0b000,
                                                                 0b00101'00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2) {
        return std::min(rs1, rs2);
    }
};
using Fmin = FloatArithmInstruction<OperFmin, FloatRegValGetter<Operands::RS1>,
                                              FloatRegValGetter<Operands::RS2>>;

struct OperFmax {
    static constexpr const char* name = "fmax.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_FP, 0b001,
                                                                 0b00101'00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2) {
        return std::max(rs1, rs2);
    }
};
using Fmax = FloatArithmInstruction<OperFmax, FloatRegValGetter<Operands::RS1>,
                                              FloatRegValGetter<Operands::RS2>>;

} // namespace rvi::rv32f

