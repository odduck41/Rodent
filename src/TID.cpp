#include "TID.hpp"

#include <algorithm>

#include "exceptions.hpp"


TID::TID() {
    current = new Scope;
    current->parent = nullptr;
}

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

void TID::push(const Type& type, const Token& name) const {
    if (current->variables.contains({name.content, type})) throw redeclaration(name);
    current->variables.insert({name.content, type});
}


Type TID::used(const Token& name) const {
    auto now = current;
    while (now != nullptr) {
        if (const auto ptr =
            std::find_if(now->variables.begin(), now->variables.end(),
                [=](const Variable& x){return name.content == x.first;});
            ptr != now->variables.end())
            return ptr->second;
        now = now->parent;
    }
    throw undefined(name);
}
