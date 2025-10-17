#pragma once

#include "rvi_datatypes.h"

#include <cstddef>
#include <cstring>
#include <vector>

namespace rvi {

class Memory {
    public:
        Memory() {}

        template <typename T>
        T get(Address address) const {
            T value = {};
            std::memcpy(&value, &memory_[address], sizeof(T));
            return value;
        }

        template <typename T>
        T set(Address address, T value) {
            std::memcpy(&memory_[address], &value, sizeof(T));
            return value;
        }

    private:
        static constexpr size_t MEM_SIZE_ = 1UL << sizeof(Address)*8; // NOTE: 4 GB vector
        std::vector<std::byte> memory_{MEM_SIZE_};
};

} // namespace rvi

