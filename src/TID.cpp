#include "TID.hpp"
#include "exceptions.hpp"

void TID::next_scope() {
    const auto nw = new Node;
    nw->parent = current;
    current = nw;
}

void TID::exit_scope() {
    const auto nw = current;
    current = current->parent;
    delete nw;
}

std::set<TID::Variable>::iterator TID::exists(const Variable& var) const {
    return current->variables.find(var);
}

void TID::add(const Variable& var) const {
    if (auto other = exists(var); other != current->variables->end()) throw redeclaration(var.name, other->line, var.line);
    current->variables.insert(var);
}

void TID::used(const Variable& var) const {
    if (exists(var) == current->variables->end()) throw undeclared(var.name, var.line);
}



