#pragma once
#include <string>
#include <set>

#include "basic.hpp"

using type = std::wstring;
using name = std::wstring;

using variable = std::pair<name, type>;

class TID {
public:
    TID() = default;

    void nextScope();
    void exitScope();

    void push(const Token&, const Token&) const;
    type used(const Token&) const;
private:
    struct Scope {
        Scope* parent = nullptr;
        std::set<variable> variables;
    };

    Scope* current{};
};
