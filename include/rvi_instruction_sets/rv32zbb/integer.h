#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

#include <algorithm>
#include <bit>

namespace rvi::rv32zbb {

struct OperMax {
    static constexpr const char* name = "max";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b110, 0b0000101);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        SignValue res = std::max(std::bit_cast<SignValue>(rs1), std::bit_cast<SignValue>(rs2));
        return std::bit_cast<UnsignValue>(res);
    }
};
using Max = ArithmInstruction<OperMax, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperMaxu {
    static constexpr const char* name = "maxu";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b111, 0b0000101);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return std::max(rs1, rs2);
    }
};
using Maxu = ArithmInstruction<OperMaxu, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperMin {
    static constexpr const char* name = "min";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b100, 0b0000101);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        SignValue res = std::min(std::bit_cast<SignValue>(rs1), std::bit_cast<SignValue>(rs2));
        return std::bit_cast<UnsignValue>(res);
    }
};
using Min = ArithmInstruction<OperMin, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperMinu {
    static constexpr const char* name = "minu";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b101, 0b0000101);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return std::min(rs1, rs2);
    }
};
using Minu = ArithmInstruction<OperMinu, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperSextb {
    static constexpr const char* name = "sext.b";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_5_7(PlainOpcodes::OP_IMM, 0b001,
                                                                   0b00100, 0b0110000);

    static UnsignValue evaluate(UnsignValue rs1) noexcept {
        if ((rs1 >> 7) & 1u)
            return rs1 | 0xFFFF'FF00;
        else
            return rs1 & 0x0000'00FF;
    }
};
using Sextb = ArithmInstruction<OperSextb, RegValGetter<Operands::RS1>>;

struct OperSexth {
    static constexpr const char* name = "sext.h";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_5_7(PlainOpcodes::OP_IMM, 0b001,
                                                                   0b00101, 0b0110000);

    static UnsignValue evaluate(UnsignValue rs1) noexcept {
        if ((rs1 >> 15) & 1u)
            return rs1 | 0xFFFF'0000;
        else
            return rs1 & 0x0000'FFFF;
    }
};
using Sexth = ArithmInstruction<OperSexth, RegValGetter<Operands::RS1>>;

struct OperZext {
    static constexpr const char* name = "zext";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_5_7(PlainOpcodes::OP, 0b100,
                                                                   0b00000, 0b0000100);

    static UnsignValue evaluate(UnsignValue rs1) noexcept {
        return rs1 & 0x0000'FFFF;
    }
};
using Zext = ArithmInstruction<OperZext, RegValGetter<Operands::RS1>>;

} // namespace rvi::rv32zbb

