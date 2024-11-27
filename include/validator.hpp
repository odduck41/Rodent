#pragma once

#include <stack>
#include <string>
#include <map>

#include "basic.hpp"

// Я ебал это писать
inline std::map<std::wstring, std::map<std::wstring, std::wstring> > transformations = {
    {
        L"int",
        {
            {L"char", L"int"},
            {L"double", L"double"},
            {L"bool", L"int"},
            {L"int", L"int"}
        }
    },

    {
        L"char",
        {
            {L"int", L"int"},
            {L"double", L"double"},
            {L"bool", L"char"},
            {L"char", L"str"},
            // {L"str", L"str"}
        }
    },

    {
        L"double",
        {
            {L"int", L"double"},
            {L"double", L"double"},
            {L"bool", L"double"},
            {L"char", L"double"}
        }
    },

    {
        L"bool",
        {
            {L"int", L"int"},
            {L"double", L"double"},
            {L"bool", L"bool"},
            {L"char", L"char"},
        }
    },

    // {L"str",
    //     {
    //         {L"str", L"str"},
    //         {L"char", L"str"}
    //     }
    // }
};

struct Element {
    SemUnit unit;

    virtual ~Element() = default;
};

enum class Val {
    lvalue, rvalue
};

struct Operation final : Element {
    Val value{0};
    enum class Type {
        unary, binary
    };

    Type type{};
};

struct Variable final : Element {
    Val value{0};
    std::wstring type{};

    Variable() = default;

    Variable(const SemUnit&, Val, std::wstring );
};

class SemStack {
public:
    SemStack() = default;

    void checkBin(); // 3 -> 1
    void checkUno(); // 2 -> 1
    void push(const Operation&);
    void push(const Variable&);
    std::wstring topType();
    std::wstring topOperation();
    void pop();
private:
    std::stack<Element*> elements_{};
};
