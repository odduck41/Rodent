#pragma once

#include <stack>
#include <string>

#include "basic.hpp"

struct Element {
    size_t line{};
    std::wstring content{};
    virtual ~Element() = default;
};

struct Operation final : Element {
    enum class Val {
        rvalue, lvalue,
    };
    Val result{};
};

struct Value final : Element {
    [[nodiscard]] std::wstring rvalue() const;
    [[nodiscard]] std::wstring lvalue() const;

    std::wstring type{};
};

class Semantic {
public:
    void checkBin();
    void checkUno();
    Semantic() = default;
    Element* push(const Token&, Operation::Val); // usually for operations
    Element* push(const std::wstring&, size_t); // usually for functions and variables
    Type top();
private:
    Element* pushOperation(const Token&, Operation::Val);
    std::stack<Element*> elements;
};
