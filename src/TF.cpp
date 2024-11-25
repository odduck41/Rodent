#include "TF.hpp"
#include "exceptions.hpp"

TF::TF() {
    current = new Node;
    current->parent = nullptr;
}

TF::~TF() {
    current = new Node;
    current->parent = nullptr;
}

void TF::next_scope() {
    const auto nw = new Node;
    nw->parent = current;
    current = nw;
}

void TF::exit_scope() {
    const auto nw = current;
    current = current->parent;
    delete nw;
}

std::set<TF::Function>::iterator TF::exists(const Function& var, Node* scope) const {
    if (scope == nullptr) {
        scope = current;
    }
    return scope->functions.find(var);
}

void TF::used(const Function& func) const {
    auto nw = current;
    while (nw != nullptr && exists(func, nw) == nw->functions.end()) nw = nw->parent;
    if (nw == nullptr) {
        std::vector <std::wstring> args_types;
        for (const FunctionArgument& arg : func.arguments) {
            args_types.push_back(arg.type);
        }
        throw undeclared_function(func.name, func.line, args_types);
    }
}

void TF::add(const Function& func) const {
    current->functions.insert(func);
}
