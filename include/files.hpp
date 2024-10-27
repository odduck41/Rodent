#pragma once


#include <fstream>

class RFile : public std::wifstream {
  public:
    RFile() = delete;
    explicit RFile(const char*);
    long long size();
};
