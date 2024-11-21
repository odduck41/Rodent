#pragma once
#include <string>
#include <set>


class TID {
  public:
    struct Variable {
        std::wstring name{};
        size_t line{};
        std::wstring type{};
    };
    TID() = default;
    void next_scope();
    void exit_scope();
    void add(const Variable&) const;
    inline void used(const Variable&) const;
  private:
    [[nodiscard]] inline std::set<Variable>::iterator exists(const Variable&) const;
    auto comparator = [](const Variable& a, const Variable& b) -> bool {
        return a.name < b.name;
    };

    struct Node {
         Node* parent{};
         std::set<Variable, decltype(comparator)> variables{};
    };
    Node* current{};
};
