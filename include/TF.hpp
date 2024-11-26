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
    TF();
    ~TF();
    void next_scope();
    void exit_scope();
    void add(const Function&) const;
    void used(const Function&) const;
private:
    struct Node {
        Node* parent{};
        std::set<Function> functions{};
    };
    Node* current{};

    [[nodiscard]] inline std::set<Function>::iterator exists(const Function&, Node* = nullptr) const;
};
