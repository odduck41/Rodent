#include "TF.hpp"

#include "exceptions.hpp"

Function::Function(const Token& type, const Token& name, const std::vector<Token>& args)
    : type_(type.content), name_(name.content) {
    args_.reserve(args.size());
    for (const auto& arg: args) {
        args_.push_back(arg.content);
    }
}

Function::Function(const Token& name, const std::vector<Token>& args)
    : name_(name.content) {
    args_.reserve(args.size());
    for (const auto& arg: args) {
        args_.push_back(arg.content);
    }
}

Function::Function(const Token& name, const std::vector<Type>& args)
: name_(name.content) {
    args_.reserve(args.size());
    for (const auto& arg: args) {
        args_.push_back(arg);
    }

}

Name Function::getName() const {
    return name_;
}

Type Function::getType() const {
    return type_;
}

bool operator==(const Function& a, const Function& b) {
    return a.name_ == b.name_ && std::equal(a.args_.begin(), a.args_.end(), b.args_.begin());
}

bool operator<(const Function& a, const Function& b) {
    if (a.name_ < b.name_) return true;
    if (a.type_ < b.type_) return true;
    if (a.args_.size() < b.args_.size()) return true;
    for (size_t i = 0; i < a.args_.size(); ++i) {
        if (a.args_[i] >= b.args_[i]) return false;
    }
    return true;
}

void TF::push(const Token& type, const Token& name, const std::vector<Token>& args) {
    last_ = type.content;
    if (functions.contains({name, args})) throw redefinition(name);
    functions.insert({type, name, args});
}

Type TF::used(const Token& name, const std::vector<Type>& args) {
    if (const auto ptr = functions.find({name, args}); ptr != functions.end()) return ptr->getType();
    throw undefined(name, 0);
}

Type TF::getLastType() const {
    return last_;
}
