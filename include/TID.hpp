#pragma once
#include <string>
#include <set>

class TID {
  public:
    struct Variable {
        std::wstring name{};
        size_t line{};
        std::wstring type{};
        bool operator<(const Variable& other) const {
            return this->name < other.name;
        }
    };
    TID();
    ~TID();
    void next_scope();
    void exit_scope();
    void add(const Variable&) const;
    void used(const Variable&) const;
  private:
    struct Node {
         Node* parent{};
         std::set<Variable> variables{};
    };
    Node* current{};

    [[nodiscard]] inline std::set<Variable>::iterator exists(const Variable&, Node* = nullptr) const;
};
