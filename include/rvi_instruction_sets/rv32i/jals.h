#pragma once

#include "rvi_opcodes.h"
#include "rvi_operands.h"
#include "rvi_instructions.h"

namespace rvi {

namespace rv32i {

template <typename Oper, class... ValuesGetters>
struct JalInstruction: Instruction {
    ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) override {
        Address next_pc = state->pc.get_next();

        ExecStatus res = state->pc.set(Oper::new_pc(ValuesGetters::get_val(*state, operands)...));

        state->regs.set(RegValGetter<Operands::RD>::get_val(*state, operands), next_pc);

        return res;
    };
};

struct OperJal {
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::JAL, 0, 0};

    static Address new_pc(Address pc, UnsignValue offset) {
        return pc + static_cast<Address>(offset);
    }
};
using Jal = JalInstruction<OperJal, PCValGetter, ImmValGetter<Operands::IMM_J>>;

struct OperJalr {
    static constexpr ExtendedOpcode ext_opcode = {PlainOpcodes::JALR, 0, 0};

    static Address new_pc(UnsignValue rs1, UnsignValue offset) {
        return (rs1 + offset) & ~1u;
    }
};
using Jalr = JalInstruction<OperJalr, RegValGetter<Operands::RS1>, ImmValGetter<Operands::IMM_I>>;

} // namespace rv32i

} // namespace rvi

