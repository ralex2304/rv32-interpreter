#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

#include "config.h"

namespace rvi {

template <class Oper>
struct FloatFmaInstruction: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        state->fregs[operands.get<RegisterNum, Operands::RD>()] = Oper::evaluate(
                                    FloatRegValGetter<Operands::RS1>::get_val(*state, operands),
                                    FloatRegValGetter<Operands::RS2>::get_val(*state, operands),
                                    FloatRegValGetter<Operands::RS3>::get_val(*state, operands));

        state->pc.increment();
        return ExecStatus::SUCCESS;
    }
};

} // namespace rvi

namespace rvi::rv32f {

struct OperFmadd {
    static constexpr const char* name = "fmadd.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeRs3(PlainOpcodes::MADD, ROUND_MODE, 0b00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2, FloatValue rs3) {
        return rs1 * rs2 + rs3;
    }
};
using Fmadd = FloatFmaInstruction<OperFmadd>;

struct OperFmsub {
    static constexpr const char* name = "fmsub.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeRs3(PlainOpcodes::MSUB, ROUND_MODE, 0b00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2, FloatValue rs3) {
        return rs1 * rs2 - rs3;
    }
};
using Fmsub = FloatFmaInstruction<OperFmsub>;

struct OperFnmsub {
    static constexpr const char* name = "fnmsub.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeRs3(PlainOpcodes::NMSUB, ROUND_MODE, 0b00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2, FloatValue rs3) {
        return -rs1 * rs2 + rs3;
    }
};
using Fnmsub = FloatFmaInstruction<OperFnmsub>;

struct OperFnmadd {
    static constexpr const char* name = "fnmadd.s";
    static constexpr ExtendedOpcode ext_opcode = OpcodeRs3(PlainOpcodes::NMADD, ROUND_MODE, 0b00);

    static FloatValue evaluate(FloatValue rs1, FloatValue rs2, FloatValue rs3) {
        return -rs1 * rs2 - rs3;
    }
};
using Fnmadd = FloatFmaInstruction<OperFnmadd>;

} // namespace rvi::rv32f

