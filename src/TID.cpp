#include "TID.hpp"

#include "exceptions.hpp"

void TID::nextScope() {
    const auto parent = current;
    current = new Scope;
    current->parent = parent;
}

void TID::exitScope() {
    const auto useless = current;
    current = current->parent;
    delete useless;
}

void TID::push(const Token& t, const Token& n) const {
    if (current->variables.contains({n.content, t.content})) throw redeclaration(n);
    current->variables.insert({n.content, t.content});
}

type TID::used(const Token& n) const {
    auto now = current;
    while (now != nullptr) {
        if (const auto ptr = now->variables.find({n.content, L""}); ptr != now->variables.end())
            return ptr->second;
        now = now->parent;
    }
    throw undefined(n);
}
