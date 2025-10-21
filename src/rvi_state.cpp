#include "rvi_state.h"

#include "rvi_datatypes.h"
#include "rvi_elf.h"
#include "rvi_logger.h"

#include <bit>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <format>
#include "magic_enum_adapter.h"
#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>

using namespace rvi;

namespace {

enum RegABI {
    ZERO =  0,
    RA   =  1,
    SP   =  2,
    GP   =  3,
    TP   =  4,
    T0   =  5, T1, T2,
    FP   =  8,
    S0   =  8, S1,
    A0   = 10, A1, A2, A3, A4, A5, A6, A7,
    S2   = 18, S3, S4, S5, S6, S7, S8, S9, S10, S11,
    T3   = 28, T4, T5, T6,

    REG_ABI_LAST_REG
};
static_assert(REG_ABI_LAST_REG == REG_NUM);

} // namespace

void RegisterFile::dump() const {
    std::string str = "00: zzzzzzzz";

    for (size_t i = 1; i < 32; i++) {
        str += std::format(" {:8x}", get(i));

        if (i % 8 == 7) {
            LOG_F(DUMP, "%s", str.c_str());
            str = std::format("{:02}:", i+1);
        }
    }
}

void RviState::load_elf(const std::filesystem::path& elf_path) {
    LOG_SCOPE_F(DUMP, "Loading elf: %s", elf_path.c_str());

    ElfLoader elf(elf_path);

    elf.load_to_memory(mem);

    LOG_F(INFO, "Entry PC: 0x%x", elf.get_entry_pc());

    pc.set(elf.get_entry_pc());
}

void RviState::init_execution_environment(const std::vector<std::string>& argv) {
    LOG_SCOPE_F(INFO, "Initializing execution environment");

    UnsignValue argc = static_cast<UnsignValue>(argv.size());

    Address sp = static_cast<Address>(0 - (PAGE_SIZE * 4));
    regs.set(SP, sp);
    LOG_F(INFO, "SP: 0x%x", sp);

    LOG_F(INFO, "argc = %u", argc);

    mem.set<UnsignValue>(sp, argc);
    sp += sizeof(UnsignValue);

    Address argv_string_addr = sp + static_cast<Address>((argc + 1) * sizeof(Address));

    size_t argv_i = 0;
    for (auto arg: argv) {
        LOG_F(INFO, "argv[%zu] = \"%s\"", argv_i++, arg.c_str());

        mem.set<Address>(sp, argv_string_addr);
        sp += sizeof(Address);

        mem.memcpy(argv_string_addr, arg.c_str(), arg.size());

        mem.set<uint8_t>(argv_string_addr + static_cast<Address>(arg.size()), 0);

        argv_string_addr += static_cast<Address>(arg.size()) + 1;
    }

    mem.set<Address>(sp, 0);
    sp += sizeof(Address);
}

ExecStatus RviState::syscall() {
    enum ArgRegsNum {
        RET_VAL        = A0,
        ARG1           = A0,
        ARG2           = A1,
        ARG3           = A2,
        ARG4           = A3,
        ARG5           = A4,
        ARG6           = A5,
        SYSCALL_NUMBER = A7,
    };

    UnsignValue syscall_num = regs.get(SYSCALL_NUMBER);

    LOG_SCOPE_F(INFO, "Syscall: %s = %u", magic_enum::enum_name(
                                                static_cast<Syscalls_>(syscall_num)).data(),
                                          syscall_num);

    switch (static_cast<Syscalls_>(syscall_num)) {
        case Syscalls_::READ:
            regs.set(RET_VAL, sys_read_(std::bit_cast<SignValue>(regs.get(ARG1)),
                                        regs.get(ARG2), regs.get(ARG3)));
            return ExecStatus::SUCCESS;
        case Syscalls_::WRITE:
            regs.set(RET_VAL, sys_write_(std::bit_cast<SignValue>(regs.get(ARG1)),
                                         regs.get(ARG2), regs.get(ARG3)));
            return ExecStatus::SUCCESS;
        case Syscalls_::EXIT:
            return ExecStatus::EXIT;

        default:
            throw std::runtime_error("Invalid syscall");
    }
}

UnsignValue RviState::sys_read_(const SignValue fd, Address buf, const UnsignValue count) {
    UnsignValue symbols_read = 0;

    while (symbols_read < count) {
        uint8_t symbol = 0;

        ssize_t res = read(fd, &symbol, sizeof(uint8_t));

        if (res == 0)
            return symbols_read;

        if (res == -1)
            return static_cast<UnsignValue>(res);

        mem.set(buf + symbols_read, symbol);

        assert(res == 1);
        symbols_read++;
    }

    return symbols_read;
}

UnsignValue RviState::sys_write_(const SignValue fd, Address buf, UnsignValue count) {
    UnsignValue symbols_written = 0;

    while (symbols_written < count) {
        uint8_t symbol = mem.get<uint8_t>(buf + symbols_written);

        ssize_t res = write(fd, &symbol, sizeof(uint8_t));

        if (res == 0)
            return symbols_written;

        if (res == -1)
            return static_cast<UnsignValue>(res);

        assert(res == 1);
        symbols_written++;
    }

    return symbols_written;
}

