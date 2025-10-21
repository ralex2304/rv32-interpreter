#pragma once

#include <bit>
#include <cassert>
#include <cstdint>

namespace bit_utils {

template <typename T, uint8_t width>
inline constexpr T get_mask() {
    static_assert(width <= sizeof(T) * 8);

    return (std::bit_cast<T>(1) << width) - 1;
}

template <typename OUT_T, typename IN_T, uint8_t offset, uint8_t width>
inline constexpr OUT_T get_field(const IN_T raw) {
    static_assert(offset + width <= sizeof(IN_T) * 8);
    static_assert(width <= sizeof(OUT_T) * 8);

    IN_T mask = get_mask<IN_T, width>() << offset;
    return static_cast<OUT_T>((raw & mask) >> offset);
}

template <typename T, uint8_t msb>
inline constexpr T sign_ext(const T val) {
    assert((get_field<T, T, msb + 1, sizeof(T) * 8 - (msb + 1)>(val)) == 0);

    if (!get_field<uint8_t, T, msb, 1>(val)) {
        return val;
    }

    return val | (get_mask<T, sizeof(T) * 8 - (msb + 1)>() << (msb + 1));
}

} // namespace rvi

#include "rvi_datatypes.h"

namespace rvi {

template <typename OUT_T, uint8_t offset, uint8_t width>
inline OUT_T get_instr_field(RawInstruction raw) {
    return bit_utils::get_field<OUT_T, RawInstruction, offset, width>(raw);
}

template <uint8_t msb>
inline UnsignValue uval_sign_ext(UnsignValue val) {
    return bit_utils::sign_ext<UnsignValue, msb>(val);
}

}

