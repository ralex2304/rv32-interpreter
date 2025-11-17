#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

#include <bit>

namespace rvi::rv32zbb {

struct OperClz {
    static constexpr const char* name = "clz";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_5_7(PlainOpcodes::OP_IMM, 0b001,
                                                                   0b00000, 0b0110000);

    static UnsignValue evaluate(UnsignValue rs1) noexcept {
        return static_cast<UnsignValue>(std::countl_zero(rs1));
    }
};
using Clz = ArithmInstruction<OperClz, RegValGetter<Operands::RS1>>;

struct OperCtz {
    static constexpr const char* name = "ctz";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_5_7(PlainOpcodes::OP_IMM, 0b001,
                                                                   0b00001, 0b0110000);

    static UnsignValue evaluate(UnsignValue rs1) noexcept {
        return static_cast<UnsignValue>(std::countr_zero(rs1));
    }
};
using Ctz = ArithmInstruction<OperCtz, RegValGetter<Operands::RS1>>;

struct OperCpop {
    static constexpr const char* name = "cpop";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_5_7(PlainOpcodes::OP_IMM, 0b001,
                                                                   0b00010, 0b0110000);

    static UnsignValue evaluate(UnsignValue rs1) noexcept {
        return static_cast<UnsignValue>(std::popcount(rs1));
    }
};
using Cpop = ArithmInstruction<OperCpop, RegValGetter<Operands::RS1>>;

} // namespace rvi::rv32zbb

