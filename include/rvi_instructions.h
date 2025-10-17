#pragma once

#include "rvi_datatypes.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"
#include "rvi_state.h"

#include <stdexcept>
#include <string>

namespace rvi {

struct Instruction {
    virtual ExtendedOpcode get_extended_opcode() const noexcept = 0;

    virtual ExecStatus execute(RviState* state, const Operands& operands) = 0;
};

template <class Oper, class... ValuesGetters>
struct SimpleInstruction: Instruction {
    ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) override {
        ExecStatus status = Oper::execute(ValuesGetters::get_val(*state, operands)...);

        if (status == ExecStatus::SUCCESS)
            state->pc.increment();

        return status;
    }
};

template <class Oper>
struct UnimplementedInstruction: Instruction {
    ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState*, const Operands&) override {
        using namespace std::literals;

        throw std::runtime_error("Instruction \""s + std::string(Oper::name) + "\" is not implemented"s);
    }
};

} // namespace rvi

