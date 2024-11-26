#pragma once
#include <string>
#include <set>
#include <vector>
#include "basic.hpp"

class TF {
public:
    struct FunctionArgument {
        std::wstring type{};
    };
    struct Function {
        std::wstring name{};
        size_t line{};
        std::wstring type{};
        std::vector <FunctionArgument> arguments;
        bool operator<(const Function& other) const {
            return this->name < other.name;
        }
    };
    void add(const Function&);
    [[nodiscard]] SemUnit used(const Function&) const;
    [[nodiscard]] SemUnit type(const Function&) const;
private:
    std::set<Function> functions{};

    [[nodiscard]] inline std::set<Function>::iterator exists(const Function&) const;
};
