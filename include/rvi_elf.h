#pragma once

#include "rvi_datatypes.h"
#include "rvi_memory.h"

#include <cstddef>
#include <elf.h>
#include <filesystem>

namespace rvi {

class MmapFile {
    public:
        MmapFile(const std::filesystem::path path);
        ~MmapFile();

        std::byte* data;
        size_t size;

    private:
        int fd_;
};

class ElfLoader {
    public:
        ElfLoader(const std::filesystem::path elf_path);

        void load_to_memory(Memory& memory) const;

        Address get_entry_pc() const;

    private:
        const MmapFile file_;

        void verify_elf_header_() const;

        Elf32_Ehdr* elf_header_;
        Elf32_Phdr* prog_header_table_;
        Elf32_Shdr* sect_header_table_;
};

} // namespace rvi

