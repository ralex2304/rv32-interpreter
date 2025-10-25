#pragma once

#include "bit_utils.h"
#include "rvi_datatypes.h"

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <variant>

namespace rvi {

namespace impl {

template<typename Tuple>
struct TupleToVariant;

template<typename... Types>
struct TupleToVariant<std::tuple<Types...>> {
    using type = std::variant<Types...>;
};

} // impl

template<typename Key>
struct ExtendedOpcodeHasher {
    std::size_t operator()(const Key& k) const {
        return std::hash<RawInstruction>()(RawInstruction(k));
    }
};

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

class PlainOpcode {
public:
    constexpr PlainOpcode(PlainOpcodes plain_opcode)
        : opcode_(plain_opcode) {}

    constexpr PlainOpcode(RawInstruction raw_instr)
        : opcode_(static_cast<PlainOpcodes>(get_instr_field<uint8_t, 0, 7>(raw_instr))) {}

    constexpr operator RawInstruction() const {
        return static_cast<RawInstruction>(opcode_);
    }

private:
    const PlainOpcodes opcode_;
};

class OpcodeFunct_3: public PlainOpcode {
public:
    constexpr OpcodeFunct_3(PlainOpcodes plain_opcode, uint8_t funct3)
        : PlainOpcode(plain_opcode)
        , funct3_(funct3) {}

    OpcodeFunct_3(RawInstruction raw_instr)
        : PlainOpcode(raw_instr)
        , funct3_(get_instr_field<uint8_t, 12, 3>(raw_instr)) {}

    operator RawInstruction() const {
        return (static_cast<RawInstruction>(funct3_) << 12) |
               PlainOpcode::operator RawInstruction();
    }

private:
    const uint8_t funct3_;
};

class OpcodeFunct_3_7: public OpcodeFunct_3 {
public:
    constexpr OpcodeFunct_3_7(PlainOpcodes plain_opcode, uint8_t funct3, uint8_t funct7)
        : OpcodeFunct_3(plain_opcode, funct3)
        , funct7_(funct7) {}

    OpcodeFunct_3_7(RawInstruction raw_instr)
        : OpcodeFunct_3(raw_instr)
        , funct7_(get_instr_field<uint8_t, 25, 7>(raw_instr)) {}

    operator RawInstruction() const {
        return (static_cast<RawInstruction>(funct7_) << 25) |
               OpcodeFunct_3::operator RawInstruction();
    }

private:
    const uint8_t funct7_;
};

using ExtendedOpcodeTuple = std::tuple<PlainOpcode, OpcodeFunct_3, OpcodeFunct_3_7, RawInstruction>;

using ExtendedOpcode = typename impl::TupleToVariant<ExtendedOpcodeTuple>::type;

class ExtendedOpcodesFactory {
public:
    static constexpr ExtendedOpcodeTuple operator()(RawInstruction raw_instr) noexcept {
        return TupleFiller_<ExtendedOpcodeTuple>::fill_with_same(raw_instr);
    }

private:
    template<typename Tuple>
    struct TupleFiller_;

    template<typename... Types>
    struct TupleFiller_<std::tuple<Types...>> {
        static constexpr std::tuple<Types...> fill_with_same(RawInstruction value) {
            return std::make_tuple(Types(value)...);
        }
    };
};

} // namespace rvi

