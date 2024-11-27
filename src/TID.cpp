#include "TID.hpp"
#include "exceptions.hpp"

#include <stdexcept>

TID::TID() {
    current = new Node;
    current->parent = nullptr;
}

TID::~TID() {
    delete current;;
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

std::wstring TID::used(const std::wstring& name, const size_t line) const {
    const Variable var{name, 0, L""};
    auto nw = current;
    while (nw != nullptr && exists(var, nw) == nw->variables.end()) {
        nw = nw->parent;
    }
    if (nw == nullptr) {
        throw undeclared(var.name, line);
    }
    auto ex = exists(var, nw);
    return ex->type;
}
