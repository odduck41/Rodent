#pragma once
#include <string>
#include <set>
#include <vector>

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
    void used(const Function&) const;
    [[nodiscard]] std::wstring type(const Function&) const;
private:
    std::set<Function> functions{};

    [[nodiscard]] inline std::set<Function>::iterator exists(const Function&) const;
};
