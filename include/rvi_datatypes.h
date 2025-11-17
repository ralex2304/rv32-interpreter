#pragma once

#include <cstddef>
#include <cstdint>

namespace rvi {

using RawInstruction = uint32_t;

using UnsignValue = uint32_t;
using SignValue   = int32_t;

using FloatValue  = float;

using Address     = UnsignValue;
using Register    = UnsignValue;
using Immediate   = UnsignValue;

using RegisterNum = uint8_t;

const size_t REG_NUM = 32;

const size_t FLOAT_REG_NUM = 32;

const size_t PAGE_SIZE = 4096;

enum class ExecStatus {
    SUCCESS                         = 0,
    EXIT                            = 1,
    INSTRUCTION_ADDRESS_MISALIGNED  = 2,
    ILLEGAL_INSTRUCTION             = 3,
    ENVIRONMENT_CALL                = 4,
    BREAKPOINT                      = 5,
};

} // namespace rvi

