#include "parser.hpp"
#include "common.hpp"
#include "exceptions.hpp"
#include <cstdlib>
#include "TID.hpp"
#include "TF.hpp"
#include "semantic.hpp"
#include <set>

TID variables;
TF functions;
Semantic expressions;

std::set<std::wstring> imported_;

bool Parser::get_() {
    ++now_;
    if (now_ >= program.size()) return false;
    now = program[now_];
    return true;
}

void Parser::get() {
    if (!get_()) throw std::logic_error("Program is unfinished!");

}

Parser::Parser(const std::vector<Token>& t, const std::string& filename) {
    const std::wstring tmp(filename.begin(), filename.end());
    filename_ = tmp;
    program = t;
    program_();
}

void Parser::program_() {
    while (get_()) {
        programThings_();
    }
}

void Parser::programThings_() {
    if (now.type == Lexeme::Type) {
        definition_();
    } else if (now.type == Lexeme::Reserved) {
        if (now.content == L"import") {
            import_();
        } else if (now.content == L"func") {
            functionDefinition_();
        } else {
            throw bad_lexeme(now, filename_);
        }
    } else {
        throw bad_lexeme(now, filename_);
    }
}

void Parser::import_() {
    get();
    if (now.type == Lexeme::StringLiteral) {
        if (imported_.contains(now.content)) return;
        imported_.insert(now.content);

        const size_t len = wcstombs(nullptr, now.content.c_str(), 0) + 1;
        const auto buffer = new char[len];

        wcstombs(buffer, now.content.c_str(), len);

        const std::string filename(buffer);

        delete[] buffer;

        doAll(filename);

    } else {
        throw bad_lexeme(now, filename_);
    }
}

void Parser::functionDefinition_() {
    get();
    if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);

    auto name = now;

    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();

    variables.nextScope();
    std::vector<Token> args;
    if (now.type != Lexeme::CloseParentheses) {
        arguments_(args);
    }

    get();
    if (now.type != Lexeme::Operation || now.content != L"->") throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::Type) throw bad_lexeme(now, filename_);

    functions.push(now, name, args);

    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);

    body_();
    variables.exitScope();
}

void Parser::arguments_(std::vector<Token>& types) {
    if (now.type != Lexeme::Type) throw bad_lexeme(now, filename_);
    auto type = now;
    types.push_back(now);
    get();
    if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);
    variables.push(type, now);
    get();
    if (now.content == L",") {
        get();

        arguments_(types);
    }

    if (now.type == Lexeme::CloseParentheses) return;
    throw bad_lexeme(now, filename_);
}

void Parser::body_() {
    get();
    if (now.type == Lexeme::CloseCurly) return;
    while (now.type != Lexeme::CloseCurly) {
        statement_();
    }
}

void Parser::inline_body_() {
    body_();
    get();
}

void Parser::statement_() {
    if (now.type == Lexeme::Semicolon) {
        get();
        return;
    }
    if (now.type == Lexeme::Reserved) {
        if (now.content == L"if") if_();
        else if (now.content == L"switch") switch_();
        else if (now.content == L"while") while_();
        else if (now.content == L"do") doWhile_();
        else if (now.content == L"for") for_();
        else if (now.content == L"return") return_();
        else if (now.content == L"continue" || now.content == L"break") {
            get();
            if (now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
            get();
        } else throw bad_lexeme(now, filename_);
    } else if (now.type == Lexeme::Type) {
        definition_();
    } else if (now.type != Lexeme::Other) {
        expression_();
    } else throw bad_lexeme(now, filename_);
}

void Parser::inline_expression() {
    expr1_();
}

void Parser::switch_() {
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);
    get();
    if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);
    const auto type = variables.used(now);

    if (type == L"array") throw bad_type(now);

    get();
    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);
    get();
    while (now.type != Lexeme::CloseCurly) {
        if (now.type != Lexeme::Reserved) throw bad_lexeme(now, filename_);
        if (now.content == L"case") {
            case_(type);
        } else {
            default_();
            break;
        }
    }
    get();
}

void Parser::case_(const Type& type) {
    get();
    if (now.type != Lexeme::Literal && now.type != Lexeme::StringLiteral) throw bad_lexeme(now, filename_);
    if (now.type == Lexeme::Literal && type == L"str") throw bad_type(now);
    if (now.type == Lexeme::StringLiteral && type != L"str") throw bad_type(now);

    get();
    if (now.type != Lexeme::Operation || now.content != L":") throw bad_lexeme(now, filename_);
    get();
    case_body_();
}

void Parser::case_body_() {
    while (now.content != L"case" && now.content != L"default") {
        statement_();
    }
}

void Parser::default_() {
    get();
    if (now.type != Lexeme::Operation || now.content != L":") throw bad_lexeme(now, filename_);
    get();
    while (now.type != Lexeme::CloseCurly) {
        statement_();
    }
}



void Parser::doWhile_() {
    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);
    inline_body_();
    if (now.type != Lexeme::Reserved && now.content != L"while") throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();
    expr_();

    if (!isComingDown(expressions.top(), L"bool")) throw bad_type(expressions.top(), now.line);
    expressions.pop();

    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
    get();
    if (now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
    get();
}

void Parser::for_() {
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::Semicolon) {
        if (now.type == Lexeme::Type) {
            definition_();
        } else {
            expr_();
        }
        if (now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
    }

    get();
    if (now.type != Lexeme::Semicolon) {
        expr_();
        if (!isComingDown(expressions.top(), L"bool")) throw bad_type(expressions.top(), now.line);
        expressions.pop();
        if (now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
    }

    get();
    if (now.type != Lexeme::CloseParentheses) {
        expr_();
        if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
    }

    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);

    inline_body_();
}

void Parser::while_() {
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();
    expr_();
    if (!isComingDown(expressions.top(), L"bool")) throw bad_type(expressions.top(), now.line);
    expressions.pop();

    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);

    inline_body_();
}

void Parser::if_() {
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();
    expr_();
    if (!isComingDown(expressions.top(), L"bool")) throw bad_type(expressions.top(), now.line);
    expressions.pop();

    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);

    inline_body_();
    if (now.type == Lexeme::Reserved && now.content == L"else") {
        get();
        if (now.type == Lexeme::Reserved && now.content == L"if") {
            if_();
            return;
        }
        if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);
        inline_body_();
        return;
    }

    if (now.type == Lexeme::Reserved && now.content == L"elif") if_();

}

void Parser::return_() {
    get();
    expression_();
    if (!isComingDown(expressions.top(), functions.getLastType()))
        throw bad_return(expressions.top(), functions.getLastType(), now.line);
    expressions.pop();
}

void Parser::definition_(std::wstring type) {
    if (type == L"#") {
        type = now.content;
    }
    if (now.content == L"array") {
        array_definition_();
        return;
    }
    get();


    if (now.type == Lexeme::Semicolon) return;

    if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);
    auto name = now;

    get();
    if (now.type != Lexeme::Punctuation
        && now.type != Lexeme::Operation
        && now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
    if (now.type == Lexeme::Semicolon) {
        variables.push(type, name);
        return;
    }
    if (now.type == Lexeme::Punctuation) {
        if (now.content != L",") throw bad_lexeme(now, filename_);
        variables.push(type, name);
        definition_(type);
        return;
    }

    if (now.type == Lexeme::Operation && now.content != L"=")
        throw bad_lexeme(now, filename_);
    get();
    inline_expression();
    if (!isComingDown(expressions.top(), type))
        throw bad_type(expressions.top(), now.line);
    expressions.pop();

    if (now.type == Lexeme::Semicolon) return;
    if (now.type == Lexeme::Punctuation) {
        if (now.content != L",") throw bad_lexeme(now, filename_);
    }
    variables.push(type, name);
    definition_(type);
}

void Parser::array_definition_() {
    get();
    if (now.content != L"<") throw bad_lexeme(now, filename_);
    get();
    if (now.type != Lexeme::Type) throw bad_lexeme(now, filename_);
    const auto valueType = now.content;
    get();
    if (now.content != L">") throw bad_lexeme(now, filename_);
    get();
    if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);
    const auto name = now;
    get();
    if (now.type == Lexeme::Semicolon) {
        variables.push(L"array<"+valueType+L">", name);
        return;
    }
    if (now.type == Lexeme::Operation && now.content != L"=") throw bad_lexeme(now, filename_);
    get();
    if (now.type == Lexeme::Operation && now.content == L"[") {
        while(now.type != Lexeme::Operation || now.content != L"]") {
            get();
            inline_expression();
            if (!isComingDown(expressions.top(), valueType)) throw bad_type(expressions.top(), now.line);
            expressions.pop();

            if (now.content == L"]") break;
            if (now.content != L",") throw bad_lexeme(now, filename_);
        }
    } else {
        inline_expression();
        if (!isComingDown(expressions.top(), L"array<"+valueType+L">"))
            throw bad_type(expressions.top(), now.line);
        expressions.pop();
    }
    get();

    variables.push(L"array<"+valueType+L">", name);
    if (now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
}

void Parser::expression_() {
    if (now.type == Lexeme::Semicolon) return;

    expr_();

    if (now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
    get();
}

void Parser::expr_() {
    expr0_();
}

void Parser::expr0_() {
    size_t amount = 0;
    expr1_();
    while (now.type == Lexeme::Punctuation) {
        if (now.content != L",") goto end;
        ++amount;
        auto e = expressions.push(now);
        get();
        expr1_();
        auto res = expressions.top();
        expressions.pop();
        if (res.back() == '&') {
            dynamic_cast<Operation*>(e)->result = Operation::Val::lvalue;
        } else {
            dynamic_cast<Operation*>(e)->result = Operation::Val::rvalue;
        }
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

bool Parser::op1(const std::wstring& s) {
    return s == L"=" ||
        s == L"+=" || s == L"-=" || s == L"*=" || s == L"/=" ||
        s == L"%=" || s == L"&=" || s == L"|=" || s == L"^=" ||
        s == L"<<=" || s == L">>=";
}

void Parser::expr1_() {
    size_t amount = 0;
    expr2_();
    while (now.type == Lexeme::Operation) {
        if (!op1(now.content)) goto end;
        ++amount;
        expressions.push(now, Operation::Val::lvalue);
        get();
        expr2_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

void Parser::expr2_() {
    size_t amount = 0;
    expr3_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"||") goto end;
        ++amount;
        expressions.push(now);
        get();
        expr3_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

void Parser::expr3_() {
    size_t amount = 0;
    expr4_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"&&") goto end;
        ++amount;
        expressions.push(now);
        get();
        expr4_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

void Parser::expr4_() {
    size_t amount = 0;
    expr5_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"|") goto end;
        ++amount;
        expressions.push(now);
        get();
        expr5_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

void Parser::expr5_() {
    size_t amount = 0;
    expr6_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"&") goto end;
        ++amount;
        expressions.push(now);
        get();
        expr6_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

void Parser::expr6_() {
    size_t amount = 0;
    expr7_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"==" && now.content != L"!=") goto end;
        ++amount;
        expressions.push(now);
        get();
        expr7_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

bool Parser::op7(const std::wstring& s) {
    return s == L">" || s == L"<" || s == L"<=" || s == L">="; // || s == L"<=>";
}

void Parser::expr7_() {
    size_t amount = 0;
    expr8_();
    while (now.type == Lexeme::Operation) {
        if (!op7(now.content))  goto end;
        ++amount;
        expressions.push(now);
        get();
        expr8_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

void Parser::expr8_() {
    size_t amount = 0;
    expr9_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L">>" && now.content != L"<<") goto end;
        ++amount;
        expressions.push(now);
        get();
        expr9_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

void Parser::expr9_() {
    size_t amount = 0;
    expr10_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"+" && now.content != L"-") goto end;
        ++amount;
        expressions.push(now);
        get();
        expr10_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

void Parser::expr10_() {
    size_t amount = 0;
    expr11_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"*" && now.content != L"/" && now.content != L"%") goto end;
        ++amount;
        expressions.push(now);
        get();
        expr11_();
    }
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    while (amount--) expressions.checkBin();
}

bool Parser::op11(const std::wstring& s) {
    return s == L"!" || s == L"++" || s == L"--" || s == L"+" || s == L"-";
}

void Parser::expr11_() {
    size_t amount = 0;
    while (now.type == Lexeme::Operation) {
        if (!op11(now.content)) goto end;
        ++amount;
        if (now.content.size() == 2) {
            expressions.push(now, Operation::Val::lvalue);
        } else {
            expressions.push(now);
        }
        get();
    }
    if (now.type != Lexeme::Identifier && now.type != Lexeme::Literal)
        throw bad_lexeme(now, filename_);
    expr12_();
    end:
    while (amount--) expressions.checkUno();
}

void Parser::expr12_() {
    size_t amount = 0;
    expr13_();
    if (now.type == Lexeme::Operation) {
        if (now.content != L"++" && now.content != L"--") goto end;
        ++amount;
        expressions.push(now);
        get();
    }
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    end:
    if (amount--) expressions.checkUno();
}

void Parser::expr13_() {
    atom();

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);

    if (now.content == L".") {
        get();
        if (now.type != Lexeme::Identifier) return;
        get();
        return;
    }

    if (now.content != L"[") return;
    get();
    expr_();

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);

    if (now.content != L"]") return;
    get();
}

void Parser::atom() {
    if (now.type != Lexeme::Identifier && now.type != Lexeme::Literal && now.type != Lexeme::StringLiteral) {
        if (now.content == L"(") get();
        while (now.content != L")") {
            inline_expression();
            if (now.content != L",") throw bad_lexeme(now, filename_);
            get();
        }
        if (now.content != L")") throw bad_lexeme(now, filename_);
    }
    if (now.type == Lexeme::Literal || now.type == Lexeme::StringLiteral) {
        expressions.push(now);
        get();
        return;
    }
    auto var = now;
    get();
    if (now.type == Lexeme::Operation || now.type != Lexeme::OpenParentheses) {
        expressions.push(variables.used(var), var.line);
        return;
    }
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    functionCall_(var);
    get();
}

void Parser::functionCall_(const Token& name) {
    std::vector<Type> args;
    get();
    given_(args);
    expressions.push(functions.used(name, args), name.line);
    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
}

void Parser::given_(std::vector<Type>& args) {
    if (now.type == Lexeme::CloseParentheses) return;
    inline_expression();
    args.push_back(expressions.top());
    expressions.pop();
    while (now.content == L",") {
        get();
        inline_expression();
        args.push_back(expressions.top());
        expressions.pop();
    }
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
}
