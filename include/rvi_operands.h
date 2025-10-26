#pragma once

#include "bit_utils.h"
#include "rvi_datatypes.h"
#include "rvi_logger.h"
#include "rvi_state.h"

#include <cstdint>
#include <format>
#include <sys/types.h>
#include <variant>

namespace rvi {

class Operands {
public:
    Operands(RawInstruction raw_instr) noexcept
        : fields_{// RD
                  get_instr_field<RegisterNum,  7, REG_FIELD_WIDTH>(raw_instr),
                  // RS1:
                  get_instr_field<RegisterNum, 15, REG_FIELD_WIDTH>(raw_instr),
                  // RS2:
                  get_instr_field<RegisterNum, 20, REG_FIELD_WIDTH>(raw_instr),
                  // IMM_I:
                  uval_sign_ext<11>( get_instr_field<Immediate, 20, 12>(raw_instr)       ),
                  // IMM_S:
                  uval_sign_ext<11>((get_instr_field<Immediate,  7,  5>(raw_instr) <<  0) |
                                    (get_instr_field<Immediate, 25,  7>(raw_instr) <<  5)),
                  // IMM_B:
                  uval_sign_ext<12>((get_instr_field<Immediate,  8,  4>(raw_instr) <<  1) |
                                    (get_instr_field<Immediate, 25,  6>(raw_instr) <<  5) |
                                    (get_instr_field<Immediate,  7,  1>(raw_instr) << 11) |
                                    (get_instr_field<Immediate, 31,  1>(raw_instr) << 12)),
                  // IMM_U:
                  /* sign_ext<31> */(get_instr_field<Immediate, 12, 20>(raw_instr) << 12),
                  // IMM_J:
                  uval_sign_ext<20>((get_instr_field<Immediate, 21, 10>(raw_instr) <<  1) |
                                    (get_instr_field<Immediate, 20,  1>(raw_instr) << 11) |
                                    (get_instr_field<Immediate, 12,  8>(raw_instr) << 12) |
                                    (get_instr_field<Immediate, 31,  1>(raw_instr) << 20))} {}

    enum Fields {
        RD          = 0,
        RS1         = 1,
        RS2         = 2,
        IMM_I       = 3,
        IMM_S       = 4,
        IMM_B       = 5,
        IMM_U       = 6,
        IMM_J       = 7,

        FIELDS_NUM,

        SHAMT       = 10,
    };

    template <typename T, Fields field>
    constexpr T get() const noexcept {
        static_assert(field < FIELDS_NUM);
        return std::get<T>(fields_[field]);
    }

    void dump() const {
        if (!Logger::is_level_needed(Logger::DUMP))
            return;

        static constexpr const char* fields_names[] = {"RD", "RS1", "RS2", "I", "S", "B", "U", "J"};

        std::string log_str;

        for (size_t i = 0; i < IMM_I; i++)
            log_str += std::format("{}: {:2} ", fields_names[i], std::get<RegisterNum>(fields_[i]));

        for (size_t i = IMM_I; i < FIELDS_NUM; i++)
            log_str += std::format("{}: {:8x} ", fields_names[i], std::get<Immediate>(fields_[i]));

        LOG_F(DUMP, "%s", log_str.c_str());
    }

private:
    const std::variant<RegisterNum, Immediate> fields_[FIELDS_NUM];

    static constexpr uint8_t REG_FIELD_WIDTH = 5;
};

template <Operands::Fields field>
struct RegValGetter {
    static_assert(Operands::RD <= field && field <= Operands::RS2);

    static UnsignValue get_val(const RviState& state, const Operands& operands) {
        return state.regs.get(operands.get<RegisterNum, field>());
    };
};

template <Operands::Fields field>
struct ImmValGetter {
    static_assert((Operands::IMM_I <= field && field <= Operands::IMM_J) ||
                  field == Operands::SHAMT);

    static UnsignValue get_val(const RviState&, const Operands& operands) {
        if constexpr (field == Operands::SHAMT)
            return operands.get<RegisterNum, Operands::RS2>();
        else 
            return operands.get<Immediate, field>();
    }
};

struct RviStateRefGetter {
    static RviState& get_val(RviState& state, const Operands&) {
        return state;
    };
};

struct PCValGetter {
    static Address get_val(const RviState& state, const Operands&) {
        return state.pc.get();
    }
};

} // namespace rvi

