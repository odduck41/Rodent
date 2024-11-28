#include "semantic.hpp"

#include "exceptions.hpp"

inline bool isLvalue(const std::wstring& value) {
    return value[value.size() - 1] == '&';
}

inline bool isRvalue(const std::wstring& value) {
    return value[value.size() - 1] != '&';
}

std::wstring Value::rvalue() const {
    if (isRvalue(type)) return type;
    return type + L"&";
}

std::wstring Value::lvalue() const {
    if (isLvalue(type)) return type;
    return type.substr(0, type.size() - 1);
}

bool requireLvalue(const std::wstring& op) {
    return (op == L"=" || (op.size() == 2 && (op[1] == '=') && (op[0] != '>' && op[0] != '<' && op[0] != '=')));
}

void Semantic::checkBin() {
    const auto a = dynamic_cast<Value*>(elements.top());
    elements.pop();
    const auto op = dynamic_cast<Operation*>(elements.top());
    elements.pop();
    const auto b = dynamic_cast<Value*>(elements.top());
    elements.pop();
    if (a == nullptr ||
        b == nullptr ||
        op == nullptr
    ) throw bad_operator(a, op, b);

    if (requireLvalue(op->content) && !isLvalue(a->type)) throw

}

Element* Semantic::push(const Token& token, const Operation::Val v = Operation::Val::rvalue) {
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

Element* Semantic::push(const std::wstring& type, const size_t line) {
    const auto variable = new Value;
    variable->type = type + L"&";
    variable->line = line;

    elements.push(variable);
    return variable;
}

Type Semantic::top() {
    return dynamic_cast<Value*>(elements.top())->type;
}

Element* Semantic::pushOperation(const Token& operation, const Operation::Val v) {
    const auto op = new Operation;
    op->content = operation.content;
    op->result = v;
    op->line = operation.line;
    elements.push(op);
    return op;
}

