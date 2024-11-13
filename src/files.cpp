#include "files.hpp"
#include <locale>
#include <codecvt>

RFile::RFile(const char* filename) : std::wifstream(filename, std::ifstream::binary) {
    this->imbue(std::locale(this->getloc(), new std::codecvt_utf8<wchar_t>));
}

long long RFile::size() {
    if (!this->is_open()) return 0;

    this->seekg(0, end);
    const long long size_ = this->tellg();
    this->seekg(0, beg);

    return size_;
}

WFile::WFile(const char* filename) : std::wofstream(filename, std::ifstream::binary) {
    this->imbue(std::locale(this->getloc(), new std::codecvt_utf8<wchar_t>));
}
