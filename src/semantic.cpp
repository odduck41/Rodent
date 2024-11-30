#include "semantic.hpp"

#include "exceptions.hpp"

inline bool isLvalue(const std::wstring& value) {
    return value[value.size() - 1] == '&';
}

inline bool isRvalue(const std::wstring& value) {
    return value[value.size() - 1] != '&';
}

std::wstring Value::lvalue() const {
    if (isLvalue(type)) return type;
    return type + L"&";
}

std::wstring Value::rvalue() const {
    if (isRvalue(type)) return type;
    return type.substr(0, type.size() - 1);
}

bool requireLvalue(const std::wstring& op) {
    return (op == L"=" || (op.size() == 2 && (op[1] == '=') && (op[0] != '>' && op[0] != '<' && op[0] != '=')));
}

void Semantic::checkBin() {
    const auto b = dynamic_cast<Value*>(elements.top());
    elements.pop();
    const auto op = dynamic_cast<Operation*>(elements.top());
    elements.pop();
    const auto a = dynamic_cast<Value*>(elements.top());
    elements.pop();
    if (a == nullptr ||
        b == nullptr ||
        op == nullptr
    ) throw bad_operator(a, op, b);

    if (requireLvalue(op->content) && !isLvalue(a->type)) throw bad_value(a);

    if (!transformations.contains(a->rvalue())
        || !transformations[a->rvalue()].contains(b->rvalue())) {
        throw wrong_operands(a, b);
    }

    const auto element = new Value;
    element->content = a->content + op->content + b->content;
    element->type = transformations[a->rvalue()][b->rvalue()];
    element->line = op->line;
    if (op->result == Operation::Val::lvalue) {
        element->type += L"&";
    }
    elements.push(element);
    delete a;
    delete b;
    delete op;
}

void Semantic::checkUno() {
    auto a = elements.top();
    elements.pop();
    auto op = elements.top();
    elements.pop();

    bool swapped = false;
    if (dynamic_cast<Operation*>(a) != nullptr) {
        std::swap(a, op);
        swapped = true;
    }
    if (op->content.size() == 2) {
        if (!swapped && !isLvalue(dynamic_cast<Value*>(a)->type)) throw bad_value(a);
    }
    if (dynamic_cast<Value*>(a)->type.contains(L"array")
        || dynamic_cast<Value*>(a)->type == L"str")
        throw wrong_operands(a);

    const auto element = new Value;
    if (swapped) {
        element->content = a->content + op->content;
        element->type = dynamic_cast<Value*>(a)->rvalue();
    } else {
        element->content = op->content + a->content;
        element->type = dynamic_cast<Value *>(a)->lvalue();
    }
    element->line = op->line;

    elements.push(element);

    delete a;
    delete op;
}

Element* Semantic::push(const Token& token, const Operation::Val v) {
    if (token.type == Lexeme::Operation) return pushOperation(token, v);
    const auto literal = new Value;
    literal->content = token.content;
    if (token.type == Lexeme::Literal) {
        literal->type = L"int";
    } else {
        literal->type = L"str";
    }
    elements.push(literal);
    return literal;
}

Element* Semantic::push(const Type& type, const size_t line, const std::wstring& name) {
    const auto variable = new Value;
    variable->type = type + L"&";
    variable->line = line;
    variable->content = name;

    elements.push(variable);
    return variable;
}

Type Semantic::top() {
    return dynamic_cast<Value*>(elements.top())->type;
}

void Semantic::pop() {
    const auto ptr = elements.top();
    elements.pop();
    delete ptr;
}

Element* Semantic::pushOperation(const Token& operation, const Operation::Val v) {
    const auto op = new Operation;
    op->content = operation.content;
    op->result = v;
    op->line = operation.line;
    elements.push(op);
    return op;
}

