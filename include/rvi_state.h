#pragma once

#include "rvi_memory.h"
#include "rvi_datatypes.h"

#include <array>
#include <cstddef>
#include <filesystem>

namespace rvi {

class ProgramCounter {
    public:
        static constexpr size_t IALIGN = 32;

        Address get() const noexcept {
            return value_;
        }

        Address get_next() const noexcept {
            return get() + sizeof(RawInstruction);
        }

        void increment() noexcept {
            value_ = get_next();
        }

        ExecStatus set(Address value) {
            value_ = value;

            // Exception must be reported on a target instruction
            return check_alignment_();
        }

    private:
        Address value_ = 0;

        ExecStatus check_alignment_() {
            if ((get() * 8) % IALIGN != 0)
                return ExecStatus::INSTRUCTION_ADDRESS_MISALIGNED;

            return ExecStatus::SUCCESS;
        }
};

class RegisterFile {
    public:
        inline Register get(size_t index) const {
            if (index == 0)
                return 0;

            return regs_[index - 1];
        }

        inline Register set(size_t index, Register value) {
            if (index == 0)
                return 0;
            return regs_[index - 1] = value;
        }

    private:
        std::array<UnsignValue, REG_NUM - 1> regs_;
};

class RviState {
    public:
        ProgramCounter pc = {};

        RegisterFile regs = {};

        Memory mem = {};

        ExecStatus syscall();

    private:
        void load_elf_(const std::filesystem::path filename);

        UnsignValue sys_read_(SignValue fd, Address buf, UnsignValue count);

        UnsignValue sys_write_(SignValue fd, Address buf, UnsignValue count);

        enum class Syscalls_ {
            READ  = 63,
            WRITE = 64,
            EXIT  = 93,
        };
};

} // namespace rvi
