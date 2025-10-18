#pragma once

#include <cstddef>
#include <cstdint>

namespace rvi {

typedef uint32_t RawInstruction;

typedef uint32_t UnsignValue;
typedef int32_t  SignValue;

typedef UnsignValue Address;
typedef UnsignValue Register;
typedef UnsignValue Immediate;

typedef uint8_t RegisterNum;

const size_t REG_NUM = 32;

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

