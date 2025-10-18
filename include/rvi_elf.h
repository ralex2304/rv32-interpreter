#pragma once

#include "rvi_datatypes.h"
#include "rvi_memory.h"

#include <cstddef>
#include <filesystem>

namespace rvi {

class MmapFile {
    public:
        MmapFile(const std::filesystem::path path);
        ~MmapFile();

        std::byte* data_;
        size_t size_;

    private:
        int fd_;
};

class ElfLoader {
    public:
        ElfLoader(const std::filesystem::path elf_path)
            : file_(elf_path) {}

        void load_to_memory(Memory mem);

        Address get_entry_pc();

    private:
        const MmapFile file_;
};

} // namespace rvi
  //
