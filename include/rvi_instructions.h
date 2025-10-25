#pragma once

#include "rvi_datatypes.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"
#include "rvi_state.h"

#include <stdexcept>
#include <string>

namespace rvi {

struct Instruction {
    virtual constexpr const char* get_name() const noexcept = 0;
    virtual constexpr ExtendedOpcode get_extended_opcode() const noexcept = 0;

    virtual ExecStatus execute(RviState* state, const Operands& operands) const = 0;

    virtual ~Instruction() {}

    class execution_error: public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
};

template <class Oper, class... ValuesGetters>
struct SimpleInstruction: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        ExecStatus status = Oper::execute(ValuesGetters::get_val(*state, operands)...);

        if (status == ExecStatus::SUCCESS)
            state->pc.increment();

        return status;
    }
};

template <class Oper>
struct UnimplementedInstruction: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState*, const Operands&) const override {
        using namespace std::literals;

        throw execution_error("Instruction \""s + std::string(Oper::name) +
                              "\" is not implemented yet"s);
    }
};

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

