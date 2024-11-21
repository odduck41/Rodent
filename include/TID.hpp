#pragma once
#include <string>
#include <set>


class TID {
  public:
    using variable = std::pair<std::wstring, std::wstring>;
    TID();
    void next_scope();
    void exit_scope();
    [[nodiscard]] inline bool exists(const variable&) const;
    void add(const variable&);
  private:
    auto comparator = [](const variable& a, const variable& b) -> bool {
        if (a.first < b.first) return false;
        return a.second >= b.second;
    };

    struct Node {
         Node* parent{};
         std::set<variable, decltype(comparator)> variables{};
    };
    Node* current{};
};
