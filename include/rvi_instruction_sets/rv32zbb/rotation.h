#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_logger.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"
#include <bit>

namespace rvi::rv32zbb {

struct OperRol {
    static constexpr const char* name = "rol";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b001, 0b0110000);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return std::rotl(rs1, rs2 & 0x1f);
    }
};
using Rol = ArithmInstruction<OperRol, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperRor {
    static constexpr const char* name = "ror";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b101, 0b0110000);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return std::rotr(rs1, rs2 & 0x1f);
    }
};
using Ror = ArithmInstruction<OperRor, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperRori {
    static constexpr const char* name = "rori";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP_IMM, 0b101, 0b0110000);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue shamt) noexcept {
        if (shamt & 0x10)
            LOG_F(ERROR, "rori: shamt[5] must be 0");

        return std::rotr(rs1, static_cast<int>(shamt));
    }
};
using Rori = ArithmInstruction<OperRori, RegValGetter<Operands::RS1>, ImmValGetter<Operands::SHAMT>>;

struct OperRev8 {
    static constexpr const char* name = "rev8";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_5_7(PlainOpcodes::OP_IMM, 0b101,
                                                                   0b11000, 0b0110100);

    static UnsignValue evaluate(UnsignValue rs1) noexcept {
        return std::byteswap(rs1);
    }
};
using Rev8 = ArithmInstruction<OperRev8, RegValGetter<Operands::RS1>>;

} // namespace rvi::rv32zbb

