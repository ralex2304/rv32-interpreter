#pragma once

#include "rvi_datatypes.h"
#include "rvi_instructions.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"

namespace rvi {

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

} // namespace rvi

