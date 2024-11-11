#include "files.hpp"

RFile::RFile(const char* filename) : std::wifstream(filename, std::ifstream::binary) {}

long long RFile::size() {
    if (!this->is_open()) return 0;

    this->seekg(0, end);
    const long long size_ = this->tellg();
    this->seekg(0, beg);

    return size_;
}

WFile::WFile(const char* filename) : std::wofstream(filename, std::ifstream::binary) {}
