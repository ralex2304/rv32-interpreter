#pragma once

#include "rvi_datatypes.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

#include <bit>

namespace rvi {

namespace rv32im {

struct OperDiv {
    static constexpr const char* name = "div";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b100, 0b0000001);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        SignValue s_rs1 = std::bit_cast<SignValue>(rs1);
        SignValue s_rs2 = std::bit_cast<SignValue>(rs2);

        return std::bit_cast<UnsignValue>(s_rs1 / s_rs2);
    }
};
using Div = ArithmInstruction<OperDiv, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperDivu {
    static constexpr const char* name = "divu";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b101, 0b0000001);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 / rs2;
    }
};
using Divu = ArithmInstruction<OperDivu, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperRem {
    static constexpr const char* name = "rem";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b110, 0b0000001);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        SignValue s_rs1 = std::bit_cast<SignValue>(rs1);
        SignValue s_rs2 = std::bit_cast<SignValue>(rs2);

        return std::bit_cast<UnsignValue>(s_rs1 % s_rs2);
    }
};
using Rem = ArithmInstruction<OperRem, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperRemu {
    static constexpr const char* name = "divu";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b111, 0b0000001);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 % rs2;
    }
};
using Remu = ArithmInstruction<OperRemu, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

} // namespace rv32im

} // namespace rvi

