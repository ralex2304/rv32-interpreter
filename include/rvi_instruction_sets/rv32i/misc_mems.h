#pragma once

#include "rvi_opcodes.h"
#include "rvi_instructions.h"

namespace rvi {

namespace rv32i {

struct OperFence {
    static constexpr const char* name = "fence";
    static constexpr ExtendedOpcode ext_opcode = {{PlainOpcodes::MISC_MEM, 0, 0},
                                                  ExtendedOpcodeType::OPCODE};
};
using Fence = UnimplementedInstruction<OperFence>;

} // namespace rv32i

} // namespace rvi

