#include "parser.hpp"
#include "common.hpp"
#include "exceptions.hpp"
#include <cstdlib>
#include <set>

std::set<std::wstring> imported_;
TID variables;
TF functions;
SemStack operations;

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
    variables.next_scope();

    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();

    if (now.type != Lexeme::CloseParentheses) {
        arguments_();
    }


    get();
    if (now.type != Lexeme::Operation || now.content != L"->") throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::Type) throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);

    body_();
    variables.exit_scope();
}

void Parser::arguments_(bool t) {
    if (now.type != Lexeme::Type) throw bad_lexeme(now, filename_);
    const auto type = now;

    get();
    if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);
    const auto identifier = now;

    get();

    bool flag = false;

    if (now.content == L"=") {
        get();
        expr1_();
        flag = true;
    }


    if (t && !flag) {
        throw bad_lexeme(now, filename_);
    }

    if (now.type == Lexeme::CloseParentheses) return;

    variables.add(TID::Variable{identifier.content, identifier.line, type.content});

    if (now.content == L",") {
        get();
        arguments_(flag);
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
    variables.next_scope();
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);
    get();
    if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);
    get();
    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);
    get();
    while (now.type != Lexeme::CloseCurly) {
        if (now.type != Lexeme::Reserved) throw bad_lexeme(now, filename_);
        if (now.content == L"case") {
            case_();
        } else {
            default_();
            break;
        }
    }
    get();
    variables.exit_scope();
}

void Parser::case_() {
    get();
    if (now.type != Lexeme::Literal && now.type != Lexeme::StringLiteral) throw bad_lexeme(now, filename_);
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
    variables.next_scope();
    inline_body_();
    variables.exit_scope();
    if (now.type != Lexeme::Reserved && now.content != L"while") throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();
    expr_();

    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
    get();
    if (now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
    get();
}

void Parser::for_() {
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);
    variables.next_scope();

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
    variables.exit_scope();
}

void Parser::while_() {
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();
    expr_();

    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);
    variables.next_scope();
    inline_body_();
    variables.exit_scope();
}

void Parser::if_() {
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();
    expr_();

    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);

    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);

    variables.next_scope();
    inline_body_();
    variables.exit_scope();
    if (now.type == Lexeme::Reserved && now.content == L"else") {
        get();
        if (now.type == Lexeme::Reserved && now.content == L"if") {
            if_();
            return;
        }
        if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);
        variables.next_scope();
        inline_body_();
        variables.exit_scope();
        return;
    }

    if (now.type == Lexeme::Reserved && now.content == L"elif") if_();

}

void Parser::return_() {
    get();
    expression_();
}

void Parser::definition_(std::wstring type) {
    size_t definition_line = now.line;
    std::vector<std::wstring> definition_name;

    if (now.content == L"array") {
        array_definition_();
        return;
    }
    if (type.empty()) {
        type = now.content;
    }

    get();

    if (now.type == Lexeme::Semicolon) return;
    if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);

    auto last = now;

    get();
    if (now.type != Lexeme::Punctuation
        && now.type != Lexeme::Operation
        && now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
    if (now.type == Lexeme::Semicolon) {
        variables.add(TID::Variable{last.content, last.line, type});
        return;
    }
    if (now.type == Lexeme::Punctuation) {
        variables.add(TID::Variable{last.content, last.line, type});
        if (now.content != L",") throw bad_lexeme(now, filename_);
        definition_(type);
        return;
    }

    if (now.type == Lexeme::Operation && now.content != L"=") throw bad_lexeme(now, filename_);
    get();
    inline_expression();

    variables.add(TID::Variable{last.content, last.line, type});
    if (now.type == Lexeme::Semicolon) return;
    if (now.type == Lexeme::Punctuation) {
        if (now.content != L",") throw bad_lexeme(now, filename_);
    }
    definition_(type);
}

void Parser::array_definition_() {
    get();
    if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);

    variables.add(TID::Variable{now.content, now.line, L"array"});

    get();
    if (now.type == Lexeme::Semicolon) return;
    if (now.content == L",") {
        array_definition_();
        return;
    }
    if (now.type == Lexeme::Operation && now.content != L"=") throw bad_lexeme(now, filename_);
    get();
    if (now.type == Lexeme::Operation && now.content == L"[") {
        while(now.type != Lexeme::Operation || now.content != L"]") {
            get();
            inline_expression();
            if (now.content == L"]") break;
            if (now.content != L",") throw bad_lexeme(now, filename_);
        }
    } else {
        expr1_();
    }

    if (now.type == Lexeme::Semicolon) return;
    get();
    if (now.content == L",") {
        array_definition_();
        return;
    }

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
    expr1_();
    while (now.type == Lexeme::Punctuation) {
        if (now.content != L",") return;
        get();
        expr1_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

bool Parser::op1(const std::wstring& s) {
    return s == L"=" ||
        s == L"+=" || s == L"-=" || s == L"*=" || s == L"/=" ||
        s == L"%=" || s == L"&=" || s == L"|=" || s == L"^=" ||
        s == L"<<=" || s == L">>=";
}

void Parser::expr1_() {
    expr2_();
    while (now.type == Lexeme::Operation) {
        if (!op1(now.content)) return;
        // SemStack push =
        get();
        expr2_();
        // SemStack.push(value)
        // lvalue = dvalue; d = don't care
        // checkbin;
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

void Parser::expr2_() {
    expr3_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"||") return;
        get();
        expr3_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

void Parser::expr3_() {
    expr4_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"&&") return;
        get();
        expr4_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

void Parser::expr4_() {
    expr5_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"|") return;
        get();
        expr5_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

void Parser::expr5_() {
    expr6_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"&") return;
        get();
        expr6_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

void Parser::expr6_() {
    expr7_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"==" && now.content != L"!=") return;
        get();
        expr7_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

bool Parser::op7(const std::wstring& s) {
    return s == L">" || s == L"<" || s == L"<=" || s == L">="; // || s == L"<=>";
}

void Parser::expr7_() {
    expr8_();
    while (now.type == Lexeme::Operation) {
        if (!op7(now.content)) return;
        get();
        expr8_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

void Parser::expr8_() {
    expr9_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L">>" && now.content != L"<<") return;
        get();
        expr9_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

void Parser::expr9_() {
    expr10_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"+" && now.content != L"-") return;
        get();
        expr10_();
    }

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

void Parser::expr10_() {
    expr11_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"*" && now.content != L"/" && now.content != L"%") return;
        get();
        expr11_();
    }
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

bool Parser::op11(const std::wstring& s) {
    return s == L"!" || s == L"++" || s == L"--" || s == L"+" || s == L"-";
}

void Parser::expr11_() {
    while (now.type == Lexeme::Operation) {
        if (!op11(now.content)) break;
        get();
    }
    if (now.type != Lexeme::Identifier && now.type != Lexeme::Literal)
        throw bad_lexeme(now, filename_);
    expr12_();
}

void Parser::expr12_() {
    expr13_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"++" && now.content != L"--") break;
        get();
    }
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
}

void Parser::expr13_() {
    atom();

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);

    if (now.content == L".") {
        get();
        if (now.type != Lexeme::Identifier) return;
        // table_of_identificators.used({now.content, now.line, now.type});
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
        get();
        return;
    }

    auto last = now;

    get();
    if (now.type == Lexeme::Operation) return;
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);

    if (now.type != Lexeme::OpenParentheses) {
        variables.used(last.content, last.line);
        // SemStack.push(now.type);
        return;
    }
    // function
    functionCall_();
    get();
}

void Parser::functionCall_() {
    const auto name = now.content;
    get();
    given_();
    auto type = functions.used(TF::Function{});
    operations.push(Variable{
    });
    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
}

std::vector<std::wstring> Parser::given_() {
    if (now.type == Lexeme::CloseParentheses) return {};
    std::vector<std::wstring> types;

    expr_();

    types.push_back(operations.topType());
    operations.pop();
    while (now.content == L",") {
        get();
        expr_();
        types.push_back(operations.topType());
        operations.pop();
    }
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
    return types;
}
