#pragma once

#include "rvi_instructions.h"
#include "rvi_opcodes.h"

namespace rvi::rv32i {

template <typename Oper>
struct StoreInstruction: Instruction {
    constexpr const char* get_name() const noexcept override {
        return Oper::name;
    }

    constexpr ExtendedOpcode get_extended_opcode() const noexcept override {
        return Oper::ext_opcode;
    }

    ExecStatus execute(RviState* state, const Operands& operands) const override {
        Address base = RegValGetter<Operands::RS1  >::get_val(*state, operands);
        Immediate offset = ImmValGetter<Operands::IMM_S>::get_val(*state, operands);

        typename Oper::WriteT value = static_cast<typename Oper::WriteT>(
                                        RegValGetter<Operands::RS2>::get_val(*state, operands));

        state->mem.set(base + offset, value);

        state->pc.increment();
        return ExecStatus::SUCCESS;
    };
};

struct OperSb {
    static constexpr const char* name = "sb";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3(PlainOpcodes::STORE, 0b000);
    using WriteT = uint8_t;
};
using Sb = StoreInstruction<OperSb>;

struct OperSh {
    static constexpr const char* name = "sh";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3(PlainOpcodes::STORE, 0b001);
    using WriteT = uint16_t;
};
using Sh = StoreInstruction<OperSh>;

struct OperSw {
    static constexpr const char* name = "sw";
    static constexpr ExtendedOpcode ext_opcode = OpcodeFunct_3(PlainOpcodes::STORE, 0b010);
    using WriteT = uint32_t;
};
using Sw = StoreInstruction<OperSw>;

} // namespace rvi::rv32i

