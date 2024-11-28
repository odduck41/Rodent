#pragma once
#include <set>

#include "basic.hpp"
#include <vector>

class Function {
public:
    Function() = default;
    Function(const Token&, const Token&, const std::vector<Token>&);
    Function(const Token&, const std::vector<Token>&);
    Function(const Token&, const std::vector<Type>&);
    [[nodiscard]] Name getName() const;
    [[nodiscard]] Type getType() const;
private:
    Type type_{};
    std::vector<Type> args_{};
    Name name_{};
    friend bool operator==(const Function&, const Function&);
    friend bool operator<(const Function&, const Function&);
};

bool operator==(const Function&, const Function&);
bool operator<(const Function&, const Function&);

class TF {
public:
    TF() = default;
    void push(const Token&, const Token&, const std::vector<Token>&);
    Type used(const Token&, const std::vector<Type>&);
    [[nodiscard]] Type getLastType() const;
private:
    std::set<Function> functions{};
    Type last_;
};
