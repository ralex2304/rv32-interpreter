#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

#include <bit>
#include <cstdint>

namespace rvi::rv32m {

struct OperMul {
    static constexpr const char* name = "mul";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b000, 0b0000001);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 * rs2;
    }
};
using Mul = ArithmInstruction<OperMul, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperMulh {
    static constexpr const char* name = "mulh";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b001, 0b0000001);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        int64_t sx_rs1 = static_cast<int64_t>(std::bit_cast<SignValue>(rs1));
        int64_t sx_rs2 = static_cast<int64_t>(std::bit_cast<SignValue>(rs2));

        SignValue res = static_cast<SignValue>((sx_rs1 * sx_rs2) >> sizeof(UnsignValue)*8);

        return std::bit_cast<UnsignValue>(res);
    }
};
using Mulh = ArithmInstruction<OperMulh, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperMulhsu {
    static constexpr const char* name = "mulhsu";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b010, 0b0000001);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        int64_t sx_rs1 = static_cast<int64_t>(std::bit_cast<SignValue>(rs1));
        int64_t  x_rs2 = static_cast<int64_t>(rs2);

        SignValue res = static_cast<SignValue>((sx_rs1 * x_rs2) >> sizeof(UnsignValue)*8);

        return std::bit_cast<UnsignValue>(res);
    }
};
using Mulhsu = ArithmInstruction<OperMulhsu, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperMulhu {
    static constexpr const char* name = "mulhu";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3_7(PlainOpcodes::OP, 0b011, 0b0000001);

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        uint64_t x_rs1 = static_cast<uint64_t>(rs1);
        uint64_t x_rs2 = static_cast<uint64_t>(rs2);

        return static_cast<UnsignValue>((x_rs1 * x_rs2) >> sizeof(UnsignValue)*8);
    }
};
using Mulhu = ArithmInstruction<OperMulhu, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

} // namespace rvi::rv32im

