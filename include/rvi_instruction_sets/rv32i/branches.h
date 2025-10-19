#pragma once

#include "rvi_opcodes.h"
#include "rvi_instructions.h"

namespace rvi {

namespace rv32i {

template <typename Oper>
struct BranchInstruction: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        if (Oper::compare(RegValGetter<Operands::RS1>::get_val(*state, operands),
                          RegValGetter<Operands::RS2>::get_val(*state, operands))) {
            return state->pc.set(state->pc.get() + ImmValGetter<Operands::IMM_B>::get_val(*state, operands));
        }

        state->pc.increment();
        return ExecStatus::SUCCESS;
    };
};

struct OperBeq {
    static constexpr const char* name = "beq";
    static constexpr ExtendedOpcode ext_opcode = {{PlainOpcodes::BRANCH, 0b000, 0},
                                                  ExtendedOpcodeType::OPCODE_FUNCT_3};

    static bool compare(UnsignValue op1, UnsignValue op2) {
        return op1 == op2;
    }
};
using Beq = BranchInstruction<OperBeq>;

struct OperBne {
    static constexpr const char* name = "bne";
    static constexpr ExtendedOpcode ext_opcode = {{PlainOpcodes::BRANCH, 0b001, 0},
                                                  ExtendedOpcodeType::OPCODE_FUNCT_3};

    static bool compare(UnsignValue op1, UnsignValue op2) {
        return op1 != op2;
    }
};
using Bne = BranchInstruction<OperBne>;

struct OperBlt {
    static constexpr const char* name = "blt";
    static constexpr ExtendedOpcode ext_opcode = {{PlainOpcodes::BRANCH, 0b100, 0},
                                                  ExtendedOpcodeType::OPCODE_FUNCT_3};

    static bool compare(UnsignValue op1, UnsignValue op2) {
        return static_cast<SignValue>(op1) < static_cast<SignValue>(op2);
    }
};
using Blt = BranchInstruction<OperBlt>;

struct OperBge {
    static constexpr const char* name = "bge";
    static constexpr ExtendedOpcode ext_opcode = {{PlainOpcodes::BRANCH, 0b101, 0},
                                                  ExtendedOpcodeType::OPCODE_FUNCT_3};

    static bool compare(UnsignValue op1, UnsignValue op2) {
        return static_cast<SignValue>(op1) >= static_cast<SignValue>(op2);
    }
};
using Bge = BranchInstruction<OperBge>;

struct OperBltu {
    static constexpr const char* name = "bltu";
    static constexpr ExtendedOpcode ext_opcode = {{PlainOpcodes::BRANCH, 0b110, 0},
                                                  ExtendedOpcodeType::OPCODE_FUNCT_3};

    static bool compare(UnsignValue op1, UnsignValue op2) {
        return op1 < op2;
    }
};
using Bltu = BranchInstruction<OperBltu>;

struct OperBgeu {
    static constexpr const char* name = "bgeu";
    static constexpr ExtendedOpcode ext_opcode = {{PlainOpcodes::BRANCH, 0b111, 0},
                                                  ExtendedOpcodeType::OPCODE_FUNCT_3};

    static bool compare(UnsignValue op1, UnsignValue op2) {
        return op1 >= op2;
    }
};
using Bgeu = BranchInstruction<OperBgeu>;

} // namespace rv32i

} // namespace rvi

