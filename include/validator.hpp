#pragma once

#include <stack>
#include <string>

struct SemType {
    std::wstring content{};
};

struct Operation : SemType {
    ;
};

struct Type : SemType {
    enum class Val {
        lvalue, rvalue
    };
    Val value{0};
};


struct Element {
    std::wstring content{};
    SemType* type{};
};

class SemStack {
public:
    SemStack();
    void checkBin(); // 3 -> 1
    void checkUno(); // 2 -> 1
    void push();
private:
    std::stack<Element>;
};