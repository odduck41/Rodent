#include "TF.hpp"
#include "exceptions.hpp"


std::set<TF::Function>::iterator TF::exists(const Function& func) const {
    return functions.find(func);
}

SemUnit TF::used(const Function& func) const {
    return type(func);
}

SemUnit TF::type(const Function& func) const {
    if (const auto ptr = exists(func); ptr != functions.end()) return {ptr->type, ptr->line};
    throw undeclared_function(func.name, func.line, func.arguments);
}

void TF::add(const Function& func) {
    functions.insert(func);
}
