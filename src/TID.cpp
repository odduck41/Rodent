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

void TID::push(const Token& type, const Token& name) const {
    if (current->variables.contains({name.content, type.content})) throw redeclaration(name);
    current->variables.insert({name.content, type.content});
}

Type TID::used(const Token& name) const {
    auto now = current;
    while (now != nullptr) {
        if (const auto ptr = now->variables.find({name.content, L""}); ptr != now->variables.end())
            return ptr->second;
        now = now->parent;
    }
    throw undefined(name);
}
