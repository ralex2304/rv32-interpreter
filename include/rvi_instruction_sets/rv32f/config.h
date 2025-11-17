#pragma once

#include <cstddef>

namespace rvi::rv32f {

class illegal_instruction {};

const size_t ROUND_MODE = 0b111;
const size_t FCVTWS_ROUND_MODE = 0b001;

} // namespace rvi::rv32f

