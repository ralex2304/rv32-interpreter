#include "rvi_datatypes.h"
#include "rvi_state.h"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>

using namespace rvi;

ExecStatus RviState::syscall() {
    enum ArgRegsNum {
        RET_VAL        = 10 + /*a*/0,
        ARG1           = 10 + /*a*/0,
        ARG2           = 10 + /*a*/1,
        ARG3           = 10 + /*a*/2,
        ARG4           = 10 + /*a*/3,
        ARG5           = 10 + /*a*/4,
        ARG6           = 10 + /*a*/5,
        SYSCALL_NUMBER = 10 + /*a*/7,
    };

    switch (static_cast<Syscalls_>(regs.get(SYSCALL_NUMBER))) {
        case Syscalls_::READ:
            regs.set(RET_VAL, sys_read_(static_cast<SignValue>(regs.get(ARG1)),
                                        regs.get(ARG2), regs.get(ARG3)));
            return ExecStatus::SUCCESS;
        case Syscalls_::WRITE:
            regs.set(RET_VAL, sys_write_(static_cast<SignValue>(regs.get(ARG1)),
                                         regs.get(ARG2), regs.get(ARG3)));
            return ExecStatus::SUCCESS;
        case Syscalls_::EXIT:
            return ExecStatus::EXIT;

        default:
            throw std::runtime_error("Invalid syscall");
    }
}

void RviState::load_elf_(const std::filesystem::path) {
    // TODO: elf loading
    throw std::runtime_error("Elf loading is unimplemented yet");
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

