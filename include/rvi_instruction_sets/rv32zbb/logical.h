#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

namespace rvi::rv32zbb {

struct OperAndn {
    static constexpr const char* name = "andn";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b111, 0b0100000);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 & ~rs2;
    }
};
using Andn = ArithmInstruction<OperAndn, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperOrn {
    static constexpr const char* name = "orn";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b110, 0b0100000);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 | ~rs2;
    }
};
using Orn = ArithmInstruction<OperOrn, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperXnor {
    static constexpr const char* name = "xnor";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b100, 0b0100000);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return ~(rs1 ^ rs2);
    }
};
using Xnor = ArithmInstruction<OperXnor, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperOrcb {
    static constexpr const char* name = "orc.b";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_5_7(PlainOpcodes::OP_IMM, 0b101,
                                                                   0b00111, 0b0010100);

    static UnsignValue evaluate(UnsignValue rs1) noexcept {
        UnsignValue result = 0;
        UnsignValue byte_mask = 0xFF;

        for (size_t i = 0; i < sizeof(UnsignValue); i++, byte_mask <<= 8)
            if ((rs1 & byte_mask) != 0)
                result |= byte_mask;

        return result;
    }
};
using Orcb = ArithmInstruction<OperOrcb, RegValGetter<Operands::RS1>>;

} // namespace rvi::rv32zbb

