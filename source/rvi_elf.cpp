#include "rvi_elf.h"

#include "rvi_logger.h"

#include <cerrno>
#include <cstddef>
#include <cstring>
#include <elf.h>
#include <fcntl.h>
#include <filesystem>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace rvi;
using namespace std::literals;

MmapFile::MmapFile(const std::filesystem::path& path) {
    fd_ = open(path.c_str(), O_RDONLY);
    if (fd_ == -1)
        throw exception("Can't open file \""s + path.string() + "\": "s +
                        std::string(std::strerror(errno)));

    struct stat statbuf = {};
    if (fstat(fd_, &statbuf) == -1)
        throw exception("Can't stat file \""s + path.string() + "\": "s +
                        std::string(std::strerror(errno)));

    size = static_cast<size_t>(statbuf.st_size);

    void* mmap_res = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd_, 0);
    if (mmap_res == MAP_FAILED)
        throw exception("Can't mmap file: "s + std::string(std::strerror(errno)));

    data = static_cast<std::byte*>(mmap_res);
}

MmapFile::~MmapFile() {
    munmap(data, size);

    close(fd_);
}

ElfLoader::ElfLoader(const std::filesystem::path& elf_path)
    try : file_(elf_path) {

    if (file_.size < sizeof(Elf32_Ehdr))
        throw exception("Corrupted file: too small for ELF header");

    elf_header_ = reinterpret_cast<Elf32_Ehdr*>(file_.data);

    verify_elf_header_();

    if (file_.size < elf_header_->e_phoff + elf_header_->e_phnum * sizeof(Elf32_Phdr))
        throw exception("Corrupted file: too small for program header table");

    prog_header_table_ = reinterpret_cast<Elf32_Phdr*>(file_.data + elf_header_->e_phoff);

    if (file_.size < elf_header_->e_shoff + elf_header_->e_shnum * sizeof(Elf32_Shdr))
        throw exception("Corrupted file: too small for section header table");

    sect_header_table_ = reinterpret_cast<Elf32_Shdr*>(file_.data + elf_header_->e_shoff);

    LOG_F(DUMP, "Elf header parsed and verified");
} catch (const MmapFile::exception& e) {
    throw exception("File mapping error: "s + e.what());
}


void ElfLoader::load_to_memory(Memory& memory) const {
    Elf32_Phdr* p_hdr = prog_header_table_;
    for (size_t i = 0; i < elf_header_->e_phnum; i++, p_hdr++) {
        if (p_hdr->p_type == PT_LOAD) {
            LOG_SCOPE_F(DUMP, "Loading PT_LOAD program header.");
            LOG_F(DUMP, "vaddr 0x%x, offset 0x%x, filesz 0x%x, memsz 0x%x",
                        p_hdr->p_vaddr, p_hdr->p_offset, p_hdr->p_filesz, p_hdr->p_memsz);

            memory.memcpy(p_hdr->p_vaddr, file_.data + p_hdr->p_offset, p_hdr->p_filesz);

            if (p_hdr->p_memsz > p_hdr->p_filesz)
                memory.memset(p_hdr->p_vaddr + p_hdr->p_filesz, 0, p_hdr->p_memsz - p_hdr->p_filesz);

            memory.dump(Logger::DUMP, p_hdr->p_vaddr, p_hdr->p_filesz);
        } else if (p_hdr->p_type == PT_INTERP) {
            throw exception("dynamic linking is unsupported");
        } else if (p_hdr->p_type == PT_PHDR) {
            throw exception("PT_PHDR program header is unsupported");
        }
    }
}

Address ElfLoader::get_entry_pc() const {
    return elf_header_->e_entry;
}

void ElfLoader::verify_elf_header_() const {

    if (std::memcmp(elf_header_->e_ident + EI_MAG0, ELFMAG, SELFMAG) != 0)
        throw exception("Invalid ELF header ident");

    if (elf_header_->e_ident[EI_CLASS] != ELFCLASS32)
        throw exception("ELF must be 32 bit");

    if (elf_header_->e_ident[EI_DATA] != ELFDATA2LSB)
        throw exception("ELF must be little-endian");

    if (elf_header_->e_ident[EI_VERSION] != EV_CURRENT)
        throw exception("Invalid ELF header version");

    if (elf_header_->e_ident[EI_OSABI] != ELFOSABI_SYSV)
        throw exception("ELF OS/ABI must be UNIX - System V");

    if (elf_header_->e_ident[EI_ABIVERSION] != 0)
        throw exception("Invalid ELF ABI version");

    if (elf_header_->e_type != ET_EXEC)
        throw exception("Non executable ELF given");

    if (elf_header_->e_machine != EM_RISCV)
        throw exception("ELF machine must be RISC-V");

    if (elf_header_->e_version != EV_CURRENT)
        throw exception("Invalid ELF version");

    if (elf_header_->e_ehsize != sizeof(Elf32_Ehdr))
        throw exception("Invalid ELF header size");

    if (elf_header_->e_phentsize != sizeof(Elf32_Phdr))
        throw exception("Invalid ELF program header size");

    if (elf_header_->e_shentsize != sizeof(Elf32_Shdr))
        throw exception("Invalid ELF section header size");
}

