#pragma once


#include <fstream>

class RFile final : public std::wifstream {
  public:
    RFile() = delete;
    explicit RFile(const char*);
    long long size();
};


class WFile final : public std::wofstream {
public:
    WFile() = delete;
    explicit WFile(const char*);
};
