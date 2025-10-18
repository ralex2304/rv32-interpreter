

#include "rvi_elf.h"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

namespace rvi {

using namespace std::literals;

MmapFile::MmapFile(const std::filesystem::path path) {
    fd_ = open(path.c_str(), O_RDONLY);
    if (fd_ == -1)
        throw std::runtime_error("Can't open file \""s + path.string() + "\": "s +
                                 std::string(std::strerror(errno)));

    struct stat statbuf = {};
    if (fstat(fd_, &statbuf) == -1)
        throw std::runtime_error("Can't stat file \""s + path.string() + "\": "s +
                                 std::string(std::strerror(errno)));

    size_ = static_cast<size_t>(statbuf.st_size);

    void* mmap_res = mmap(NULL, size_, PROT_READ, MAP_PRIVATE, fd_, 0);
    if (mmap_res == MAP_FAILED)
        throw std::runtime_error("Can't mmap file: "s + std::string(std::strerror(errno)));

    data_ = static_cast<std::byte*>(mmap_res);
}

MmapFile::~MmapFile() {
    munmap(data_, size_);

    close(fd_);
}

} // namespace rvi

