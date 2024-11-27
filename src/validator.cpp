#include "validator.hpp"

#include <stdexcept>
#include <utility>

#include "exceptions.hpp"

Variable::Variable(const SemUnit& su, Val v, std::wstring  t)
: value(v), type(std::move(t))
{
    this->unit = su;
}

void SemStack::checkBin() {
    const auto a = dynamic_cast<Variable*>(elements_.top());
    elements_.pop();
    const auto op = dynamic_cast<Operation*>(elements_.top());
    elements_.pop();
    const auto b = dynamic_cast<Variable*>(elements_.top());
    elements_.pop();
    /*
     * char, char -> char;
     * double, double -> double;
     * bool, bool -> bool;
     * int, int -> int;
     * int, double -> double;
     * int, bool -> int;
     * int, char -> int;
     * bool, double -> double
     * bool, char -> char
     * char, double -> double
     * char, str -> str
     */
    if (op == nullptr || op->type != Operation::Type::binary) {
        throw bad_operator(a, op, b);
    }
    const auto aType = a->type;
    const auto bType = b->type;
    if (!transformations.contains(aType) || !transformations[aType].contains(bType)) {
        throw wrong_operands(a, b);
    }


    this->push(Variable(SemUnit{a->unit.content + op->unit.content + b->unit.content, op->unit.line},
                                  op->value,
                                  transformations[aType][bType]
    ));
    delete a;
    delete op;
    delete b;
}

void SemStack::checkUno() {
    auto a = elements_.top();
    elements_.pop();
    auto op = elements_.top();
    elements_.pop();
    bool swapped = false;
    if (dynamic_cast<Operation*>(a) == nullptr) {
        std::swap(a, op);
        swapped = true;
    }
    auto value = Val::lvalue;
    if (!swapped) {
        value = Val::rvalue;
    }
    if (dynamic_cast<Variable*>(a)->type == L"array"
        || dynamic_cast<Variable*>(a)->type == L"str")
        throw wrong_operands(dynamic_cast<Variable*>(a));
    push(Variable{
        SemUnit{a->unit.content + op->unit.content, a->unit.line},
        value,
        dynamic_cast<Variable*>(a)->type
    });
    delete a;
    delete op;
}

void SemStack::push(const Operation& x) {
    const auto op = new Operation(x);
    this->elements_.push(op);
}

void SemStack::push(const Variable& x) {
    const auto op = new Variable(x);
    this->elements_.push(op);
}

void SemStack::pop() {
    const auto ptr = elements_.top();
    elements_.pop();
    delete ptr;
}

std::wstring SemStack::topType() {
    if (dynamic_cast<Variable*>(elements_.top()) == nullptr)
        throw std::logic_error("bad interpretation");
    return dynamic_cast<Variable*>(elements_.top())->type;
}

std::wstring SemStack::topOperation() {
    if (dynamic_cast<Operation*>(elements_.top()) == nullptr)
        throw std::logic_error("bad interpretation");
    return dynamic_cast<Operation*>(elements_.top())->unit.content;
}
