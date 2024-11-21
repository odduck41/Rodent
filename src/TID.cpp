#include "TID.hpp"

#include <stdexcept>

#include "exceptions.hpp"

TID::TID() {
    current = new Node;
    current->parent = nullptr;
}

void TID::next_scope() {
    const auto nw = new Node;
    nw->parent = current;
    current = nw;
}

void TID::exit_scope() {
    const auto nw = current;
    if (current->parent == nullptr) {
        throw std::logic_error("Trying to get out from global scope");
    }
    current = current->parent;
    delete nw;
}

std::set<TID::Variable>::iterator TID::exists(const Variable& var, Node* scope) const {
    if (scope == nullptr) {
        scope = current;
    }
    return scope->variables.find(var);
}

void TID::add(const Variable& var) const {
    if (auto other = exists(var); other != current->variables.end()) throw redeclaration(var.name, other->line, var.line);
    current->variables.insert(var);
}

void TID::used(const Variable& var) const {
    auto nw = current;
    while (nw != nullptr && exists(var, nw) == nw->variables.end()) nw = nw->parent;
    if (nw == nullptr) throw undeclared(var.name, var.line);
}

// void TID::used(const Variable& var) const {
// }



