#pragma once

#include "lexer.hpp"
#include <fstream>


void starter(const std::string& filename, bool save = true) {
    load_reserved();
    load_basic_types();
    auto from = filename + ".rod";
    std::string tokens = tokenize(from.c_str());

    auto to = filename + ".dev";
    if (save) {
        std::ofstream os(to, std::ios::binary);
        os.write(tokens.c_str(), static_cast<long long>(tokens.size()));
        os.close();
    }
}
