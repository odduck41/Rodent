#pragma once
#include <string>
#include <set>
#include <vector>
#include "basic.hpp"
#include "validator.hpp"

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
            if (this->name != other.name) {
                return this->name < other.name;
            }
            if (this->arguments.size() != other.arguments.size()) {
                return this->arguments.size() < other.arguments.size();
            }
            for (int i = 0; i < this->arguments.size(); ++i) {
                if (this->arguments[i].type != other.arguments[i].type) {
                    return this->arguments[i].type < other.arguments[i].type;
                }
            }
            return false;
        }
        bool operator==(const Function& other) const {
            if (this->name != other.name) {
                return false;
            }
            if (this->arguments.size() != other.arguments.size()) {
                return false;
            }
            for (int i = 0; i < this->arguments.size(); ++i) {
                if (this->arguments[i].type != other.arguments[i].type) {
                    return false;
                }
            }
            return true;
        }
    };
    void add(const Function&);
    // [[nodiscard]] SemUnit used(const Function&) const;
    [[nodiscard]] std::wstring type(const Function&) const;
    Variable TF::used(const Token& t, const std::vector<std::wstring>&);
private:
    std::set<Function> functions{};

    [[nodiscard]] inline std::set<Function>::iterator exists(const Function&) const;
};
