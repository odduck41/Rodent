#pragma once
#include <string>
#include <set>

#include "basic.hpp"


class TID {
public:
    TID() = default;

    void nextScope();
    void exitScope();

    void push(const Token&, const Token&) const;
    [[nodiscard]] Type used(const Token&) const;
private:
    struct Scope {
        Scope* parent = nullptr;
        std::set<Variable> variables;
    };

    Scope* current{};
};
