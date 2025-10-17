#pragma once

#include "bit_utils.h"
#include "rvi_datatypes.h"

#include <array>
#include <cstddef>
#include <cstdint>

namespace rvi {

enum class PlainOpcodes: uint8_t {
    LOAD     = 0b00'000'11,
    MISC_MEM = 0b00'011'11,
    OP_IMM   = 0b00'100'11,
    AUIPC    = 0b00'101'11,

    STORE    = 0b01'000'11,
    OP       = 0b01'100'11,
    LUI      = 0b01'101'11,

    BRANCH   = 0b11'000'11,
    JALR     = 0b11'001'11,
    JAL      = 0b11'011'11,
    SYSTEM   = 0b11'100'11,
};

class ExtendedOpcode {
    public:
        constexpr ExtendedOpcode(RawInstruction full_instruction)
            : value_(full_instruction) {}

        constexpr ExtendedOpcode(PlainOpcodes plain_opcode, uint8_t funct3, uint8_t funct7)
            : value_(static_cast<RawInstruction>(plain_opcode) |
                     static_cast<RawInstruction>(funct3 << 12) |
                     static_cast<RawInstruction>(funct7 << 25)) {}

        constexpr operator RawInstruction() const noexcept {
            return value_;
        }

    private:
        const RawInstruction value_;
};

class ExtendedOpcodesFactory {
    public:
        inline ExtendedOpcodesFactory(RawInstruction raw_instr) noexcept
            : ext_opcodes_({ExtendedOpcode(decode_plain_opcode_(raw_instr), 0, 0),

                            ExtendedOpcode(decode_plain_opcode_(raw_instr),
                                           decode_funct3_(raw_instr), 0),

                            ExtendedOpcode(decode_plain_opcode_(raw_instr),
                                           decode_funct3_(raw_instr),
                                           decode_funct7_(raw_instr)),

                            ExtendedOpcode(raw_instr)}) {}

        inline auto begin() const noexcept { return ext_opcodes_.cbegin(); }
        inline auto end()   const noexcept { return ext_opcodes_.cend(); }
        constexpr size_t size() const noexcept { return ext_opcodes_.size(); }

    private:
        const std::array<ExtendedOpcode, 4> ext_opcodes_;

        static inline PlainOpcodes decode_plain_opcode_(RawInstruction raw_instr) noexcept {
            return static_cast<PlainOpcodes>(get_instr_field<uint8_t, 0, 7>(raw_instr));
        }
        static inline uint8_t decode_funct3_(RawInstruction raw_instr) noexcept {
            return get_instr_field<uint8_t, 12, 3>(raw_instr);
        }
        static inline uint8_t decode_funct7_(RawInstruction raw_instr) noexcept {
            return get_instr_field<uint8_t, 25, 7>(raw_instr);
        }
};

} // namespace rvi

