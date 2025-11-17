#pragma once

#include "rvi_datatypes.h"
#include "rvi_logger.h"

#include <array>
#include <cstddef>
#include <cstring>
#include <format>
#include <unordered_map>

namespace rvi {

class SparseMemory {
public:
    template <typename T>
    T get(Address address) {
        if (address % sizeof(T) != 0)
            return get_unaligned_<T>(address);

        auto getter = memory_.try_emplace(address / PAGE_SIZE_);
        if (getter.second)
            LOG_F(WARNING, "Memory: Reading unallocated page (addr = %x)", address);

        T value = {};
        std::memcpy(&value, &getter.first->second[address % PAGE_SIZE_], sizeof(T));

        return value;
    }

    template <typename T>
    T set(Address address, T value) {
        if (address % sizeof(T) != 0)
            return set_unaligned_<T>(address, value);

        auto page_getter = memory_.try_emplace(address / PAGE_SIZE_);

        std::memcpy(&page_getter.first->second[address % PAGE_SIZE_], &value, sizeof(T));

        return value;
    }

private:
    static constexpr Address PAGE_SIZE_ = 4096;

    std::unordered_map<Address, std::array<std::byte, PAGE_SIZE_>> memory_;

    template <typename T>
    T get_unaligned_(Address address) {
        T value = {};

        for (Address i = 0; i < sizeof(T); i++) {
            std::byte byte_val = get<std::byte>(address + i);
            std::memcpy(reinterpret_cast<std::byte*>(&value) + i, &byte_val, sizeof(std::byte));
        }

        return value;
    }

    template <typename T>
    T set_unaligned_(Address address, T value) {
        for (Address i = 0; i < sizeof(T); i++) {
            std::byte byte_val;
            std::memcpy(&byte_val, reinterpret_cast<std::byte*>(&value) + 1, sizeof(std::byte));

            set<std::byte>(address, byte_val);
        }

        return value;
    }
};

struct Memory: public SparseMemory {
    void memcpy(Address dest, const void* src, UnsignValue n) {
        const std::byte* src_ptr = reinterpret_cast<const std::byte*>(src);
        while (n-- != 0) {
            set<std::byte>(dest++, *(src_ptr++));
        }
    }

    void memset(Address addr, int val, UnsignValue n) {
        while (n-- != 0) {
            set<std::byte>(addr++, static_cast<std::byte>(val));
        }
    }

    void dump(Logger::Verbosity level, Address addr, UnsignValue size) {
        if (!Logger::is_level_needed(level))
            return;

        VLOG_SCOPE_F(level, "Memory dump: addr 0x%x; size 0x%x", addr, size);

        std::string str = "Empty";
        for (UnsignValue i = 0; i < size; i++) {
            if (i % 16 == 0) {
                if (i != 0)
                    VLOG_F(level, "%s", str.c_str());

                str = std::format("0x{:5x}", addr + i);
            }

            if (i % 2 == 0)
                str += " ";

            str += std::format("{:02x}", get<uint8_t>(addr + i));
        }

        VLOG_F(level, "%s", str.c_str());
    }
};

} // namespace rvi

