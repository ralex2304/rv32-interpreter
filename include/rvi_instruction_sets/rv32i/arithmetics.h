#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

namespace rvi {

namespace rv32i {

template <class Oper, class... ValuesGetters>
struct ArithmInstruction: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        state->regs.set(operands.get<RegisterNum, Operands::RD>(),
                        Oper::evaluate(ValuesGetters::get_val(*state, operands)...));

        state->pc.increment();
        return ExecStatus::SUCCESS;
    }
};

struct OperLui {
    static constexpr const char* name = "lui";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::LUI, 0, 0};

    static UnsignValue evaluate(UnsignValue imm) noexcept {
        return imm;
    }
};
using Lui = ArithmInstruction<OperLui, ImmValGetter<Operands::IMM_U>>;

struct OperAuipc {
    static constexpr const char* name = "auipc";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::AUIPC, 0, 0};

    static UnsignValue evaluate(Address pc, UnsignValue imm) noexcept {
        return pc + imm;
    }
};
using Auipc = ArithmInstruction<OperAuipc, PCValGetter, ImmValGetter<Operands::IMM_U>>;

struct OperAddi {
    static constexpr const char* name = "addi";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP_IMM, 0b000, 0};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue imm) noexcept {
        return rs1 + imm;
    }
};
using Addi = ArithmInstruction<OperAddi, RegValGetter<Operands::RS1>, ImmValGetter<Operands::IMM_I>>;

struct OperSlti {
    static constexpr const char* name = "slti";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP_IMM, 0b010, 0};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue imm) noexcept {
        return (static_cast<SignValue>(rs1) < static_cast<SignValue>(imm)) ? 1 : 0;
    }
};
using Slti = ArithmInstruction<OperSlti, RegValGetter<Operands::RS1>, ImmValGetter<Operands::IMM_I>>;

struct OperSltiu {
    static constexpr const char* name = "sltiu";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP_IMM, 0b011, 0};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue imm) noexcept {
        return (rs1 < imm) ? 1 : 0;
    }
};
using Sltiu = ArithmInstruction<OperSltiu, RegValGetter<Operands::RS1>, ImmValGetter<Operands::IMM_I>>;

struct OperXori {
    static constexpr const char* name = "xori";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP_IMM, 0b100, 0};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue imm) noexcept {
        return rs1 ^ imm;
    }
};
using Xori = ArithmInstruction<OperXori, RegValGetter<Operands::RS1>, ImmValGetter<Operands::IMM_I>>;

struct OperOri {
    static constexpr const char* name = "ori";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP_IMM, 0b110, 0};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue imm) noexcept {
        return rs1 | imm;
    }
};
using Ori = ArithmInstruction<OperOri, RegValGetter<Operands::RS1>, ImmValGetter<Operands::IMM_I>>;

struct OperAndi {
    static constexpr const char* name = "andi";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP_IMM, 0b111, 0};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue imm) noexcept {
        return rs1 & imm;
    }
};
using Andi = ArithmInstruction<OperAndi, RegValGetter<Operands::RS1>, ImmValGetter<Operands::IMM_I>>;

struct OperSlli {
    static constexpr const char* name = "slli";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP_IMM, 0b001, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue shamt) noexcept {
        return rs1 << shamt;
    }
};
using Slli = ArithmInstruction<OperSlli, RegValGetter<Operands::RS1>, ImmValGetter<Operands::SHAMT>>;

struct OperSrli {
    static constexpr const char* name = "srli";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP_IMM, 0b101, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue shamt) noexcept {
        return rs1 >> shamt;
    }
};
using Srli = ArithmInstruction<OperSrli, RegValGetter<Operands::RS1>, ImmValGetter<Operands::SHAMT>>;

struct OperSrai {
    static constexpr const char* name = "srai";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP_IMM, 0b101, 0b0100000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue shamt) noexcept {
        return static_cast<UnsignValue>(static_cast<SignValue>(rs1) >> shamt);
    }
};
using Srai = ArithmInstruction<OperSrai, RegValGetter<Operands::RS1>, ImmValGetter<Operands::IMM_I>>;

struct OperAdd {
    static constexpr const char* name = "add";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b000, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 + rs2;
    }
};
using Add = ArithmInstruction<OperAdd, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperSub {
    static constexpr const char* name = "sub";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b000, 0b0100000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 - rs2;
    }
};
using Sub = ArithmInstruction<OperSub, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperSll {
    static constexpr const char* name = "sll";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b001, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 << (rs2 & 0x1F);
    }
};
using Sll = ArithmInstruction<OperSll, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperSlt {
    static constexpr const char* name = "slt";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b010, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return (static_cast<SignValue>(rs1) < static_cast<SignValue>(rs2)) ? 1 : 0;
    }
};
using Slt = ArithmInstruction<OperSlt, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperSltu {
    static constexpr const char* name = "sltu";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b011, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return (rs1 < rs2) ? 1 : 0;
    }
};
using Sltu = ArithmInstruction<OperSltu, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperXor {
    static constexpr const char* name = "xor";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b100, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 ^ rs2;
    }
};
using Xor = ArithmInstruction<OperXor, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperSrl {
    static constexpr const char* name = "srl";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b101, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 >> (rs2 & 0x1F);
    }
};
using Srl = ArithmInstruction<OperSrl, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperSra {
    static constexpr const char* name = "sra";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b101, 0b0100000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return static_cast<UnsignValue>(static_cast<SignValue>(rs1) >> (rs2 & 0x1F));
    }
};
using Sra = ArithmInstruction<OperSra, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperOr {
    static constexpr const char* name = "or";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b110, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 | rs2;
    }
};
using Or = ArithmInstruction<OperOr, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

struct OperAnd {
    static constexpr const char* name = "and";
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::OP, 0b111, 0b0000000};

    static UnsignValue evaluate(UnsignValue rs1, UnsignValue rs2) noexcept {
        return rs1 & rs2;
    }
};
using And = ArithmInstruction<OperAnd, RegValGetter<Operands::RS1>, RegValGetter<Operands::RS2>>;

} // namespace rv32i

} // namespace rvi
