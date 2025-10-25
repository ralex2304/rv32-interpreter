#pragma once

#include "rvi_datatypes.h"
#include "rvi_opcodes.h"
#include "rvi_operands.h"
#include "rvi_instructions.h"

namespace rvi {

namespace rv32i {

struct OperEcall {
    static constexpr const char* name = "ecall";
    static constexpr ExtendedOpcode ext_opcode = RawInstruction(0x00000000 |
                                                 static_cast<RawInstruction>(PlainOpcodes::SYSTEM));

    static ExecStatus execute(RviState& state) {
        return state.syscall();
    }
};
using Ecall = SimpleInstruction<OperEcall, RviStateRefGetter>;

struct OperEbreak {
    static constexpr const char* name = "ebreak";
    static constexpr ExtendedOpcode ext_opcode = RawInstruction(0x00100000 |
                                                 static_cast<RawInstruction>(PlainOpcodes::SYSTEM));
};
using Ebreak = UnimplementedInstruction<OperEbreak>;

} // namespace rv32i

} // namespace rvi

