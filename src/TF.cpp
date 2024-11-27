#include "TF.hpp"
#include "exceptions.hpp"


std::set<TF::Function>::iterator TF::exists(const Function& func) const {
    return functions.find(func);
}

Variable TF::used(const Token& t, const std::vector<std::wstring>& arguments) {
    Function f(t.content, t.line);
    for (auto& i : arguments) {
        f.arguments.push_back({i});
    }
    const auto type_ = type(f);
    auto value = Val::rvalue;
    if (type_.back() == '&') {
        value = Val::lvalue;
    }
    return Variable(SemUnit{t.content, t.line}, value, type(f));
}

std::wstring TF::type(const Function& func) const {
    if (const auto ptr = exists(func); ptr != functions.end()) return ptr->type;
    throw undeclared_function(func.name, func.line, func.arguments);
}

void TF::add(const Function& func) {
    functions.insert(func);
}
