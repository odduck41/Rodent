#pragma once


#include <fstream>

class RFile : public std::ifstream {
  public:
    RFile() = delete;
    explicit RFile(const char*);
    long long size();
};
