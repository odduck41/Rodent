#include "files.hpp"

RFile::RFile(const char* filename) : std::ifstream(filename, std::ifstream::binary) {}

long long RFile::size() {
    if (!this->is_open()) return 0;

    this->seekg(0, end);
    long long size_ = this->tellg();
    this->seekg(0, beg);

    return size_;
}
