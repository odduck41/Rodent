#include "TID.hpp"

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

bool TID::exists(const variable& var) const {
    return current->variables.contains(var);
}

void TID::add(const variable& var) {
    if (exists(var)) throw
}


