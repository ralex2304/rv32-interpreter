#pragma once

#include "rvi_opcodes.h"
#include "rvi_instructions.h"

namespace rvi::rv32i {

struct OperFence {
    static constexpr const char* name = "fence";
    static constexpr ExtendedOpcode ext_opcode = PlainOpcode(PlainOpcodes::MISC_MEM);
};
using Fence = UnimplementedInstruction<OperFence>;

} // namespace rvi::rv32i

