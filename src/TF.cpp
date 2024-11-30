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

bool Function::operator==(const Function& other) const {
    if (this->args_.size() != other.args_.size() || this->name_ != other.name_) return false;
    bool comingDown = true;
    for (size_t i = 0; i < this->args_.size(); ++i) {
        comingDown &= isComingDown(other.args_[i], this->args_[i]);
        if (!comingDown) break;
    }
    return comingDown;
}

bool Function::operator<(const Function& other) const {
    if (this->name_ < other.name_) return true;
    if (this->type_ < other.type_) return true;
    if (this->args_.size() != other.args_.size()) return false;
    for (size_t i = 0; i < this->args_.size(); ++i) {
        if (this->args_[i] >= other.args_[i]) return false;
    }
    return true;
}

void TF::push(const Token& type, const Token& name, const std::vector<Token>& args) {
    last_ = type.content;
    if (functions.contains({name, args})) throw redefinition(name);
    functions.insert({type, name, args});
}

Type TF::used(const Token& name, const std::vector<Type>& args) {
    if (const auto ptr = std::ranges::find_if(functions, [=](const Function& a) {
        return a == Function{name, args};
    }); ptr != functions.end()) return ptr->getType();
    throw undefined(name, 0);
}

Type TF::getLastType() const {
    return last_;
}
