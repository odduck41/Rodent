#include "validator.hpp"

#include <stdexcept>
#include <utility>

#include "exceptions.hpp"

Operation::Operation(const Val& v, const SemUnit& su, const Type t)
: value(v), type(t) {
    this->unit = su;
}

Variable::Variable(const SemUnit& su, const Val v, std::wstring t)
: value(v), type(std::move(t))
{
    this->unit = su;
}

bool requireLvalue (const std::wstring& operation) {
    return (operation == L"=" ||
        (operation.size() == 2 &&
            (operation[1] == '=') &&
            (operation[0] != '>'
                && operation[0] != '<'
                && operation[0] != '=')
            ));
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
    if (requireLvalue(op->unit.content) && b->value != Val::lvalue)
        throw wrong_operands(a);

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
        value = Val::rvalue; // Лев блять пиши код я тебя умоляю
    } else {
        if (op->unit.content.size() == 2) {
            if (dynamic_cast<Variable*>(a)->value != Val::lvalue)
                throw wrong_operands(dynamic_cast<Variable*>(a));
        }
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

Operation* SemStack::push(const Operation& x) {
    const auto op = new Operation(x);
    this->elements_.push(op);
    return op;
}

Variable* SemStack::push(const Variable& x) {
    const auto var = new Variable(x);
    this->elements_.push(var);
    return var;
}

void SemStack::pop() {
    const auto ptr = elements_.top();
    elements_.pop();
    delete ptr;
}

Variable SemStack::topVariable() {
    if (dynamic_cast<Variable*>(elements_.top()) == nullptr)
        throw std::logic_error("bad interpretation");

    return *dynamic_cast<Variable*>(elements_.top());
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

SemStack::~SemStack() {
    while (!elements_.empty()) {
        const auto p = elements_.top();
        elements_.pop();
        delete p;
    }
}
