#include "validator.hpp"

#include "exceptions.hpp"

Variable::Variable(const SemUnit &, Val, const std::wstring &) {

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

    const auto var = new Variable(SemUnit{a->unit.content + op->unit.content + b->unit.content, op->unit.line},
                                  op->value,
                                  transformations[aType][bType]
    );

    this->elements_.push(var);
    delete a;
    delete op;
    delete b;
}

void SemStack::checkUno() {

}

void SemStack::push(const Operation& x) {
    const auto op = new Operation(x);
    this->elements_.push(op);
}

void SemStack::push(const Variable& x) {
    const auto op = new Variable(x);
    this->elements_.push(op);
}
