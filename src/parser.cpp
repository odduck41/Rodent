#include "parser.hpp"
#include "common.hpp"
#include "exceptions.hpp"
#include <cstdlib>
#include <set>

std::set<std::wstring> imported_;

bool Parser::get() {
    ++now_;
    if (now_ >= program.size()) return false;
    now = program[now_];
    return true;
}

Parser::Parser(const std::vector<Token>& t, const std::string& filename) {
    const std::wstring tmp(filename.begin(), filename.end());
    filename_ = tmp;
    program = t;
    program_();
}

void Parser::program_() {
    while (get()) {
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

}

void Parser::arguments_() {
    while(now.type != Lexeme::CloseParentheses) {
        if (now.type != Lexeme::Type) throw bad_lexeme(now, filename_);
        get();
        if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);
        get();
        if (now.type == Lexeme::CloseParentheses) return;
        if (now.type == Lexeme::Punctuation && now.content == L",") {
            get();
            continue;
        }
        if (now.content != L",") throw bad_lexeme(now, filename_);
        get();
        while(now.type != Lexeme::CloseParentheses) {
            if (now.type != Lexeme::Type) throw bad_lexeme(now, filename_);
            get();
            if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);
            get();
            if (now.type != Lexeme::Operation && now.content != L"=") throw bad_lexeme(now, filename_);
            get();
            if (now.type != Lexeme::Literal && now.type != Lexeme::StringLiteral) throw bad_lexeme(now, filename_);

            get();

            if (now.type == Lexeme::Punctuation && now.content == L",") {
                continue;
            }

            throw bad_lexeme(now, filename_);
        }
        break;
    }
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
        // else if (now.content == L"switch") switch_();
        else if (now.content == L"while") while_();
        else if (now.content == L"do") doWhile_();
        else if (now.content == L"for") for_();
        else if (now.content == L"return") return_();
        else throw bad_lexeme(now, filename_);
    } else if (now.type == Lexeme::Type) {
        definition_();
    } else if (now.type != Lexeme::Other) {
        expression_();
    } else throw bad_lexeme(now, filename_);
}

// void Parser::switch_() {
//
// }

void Parser::doWhile_() {
    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now, filename_);
    inline_body_();
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

    get();
    if (now.type != Lexeme::Semicolon) {
        expr_();
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
}

void Parser::while_() {
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now, filename_);

    get();
    expr_();

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
}

void Parser::definition_() {
    get();
    while (now.type != Lexeme::Semicolon) {
        if (now.type != Lexeme::Identifier) throw bad_lexeme(now, filename_);
        get();
        if (now.type != Lexeme::Punctuation
            && now.type != Lexeme::Operation
            && now.type != Lexeme::Semicolon) throw bad_lexeme(now, filename_);
        if (now.type == Lexeme::Semicolon) return;
        if (now.type == Lexeme::Punctuation) {
            if (now.content != L",") throw bad_lexeme(now, filename_);
            get();
            continue;
        }
        if (now.type == Lexeme::Operation && now.content != L"=") throw bad_lexeme(now, filename_);
        get();
        expr_();
        if (now.type == Lexeme::Semicolon) return;
        if (now.type == Lexeme::Punctuation) {
            if (now.content != L",") throw bad_lexeme(now, filename_);
            get();
        }
    }
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
    while (now.type == Lexeme::Operation) {
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
        get();
        expr2_();
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
        if (!op11(now.content)) return;
        get();
    }
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    expr12_();
}

void Parser::expr12_() {
    expr13_();
    while (now.type == Lexeme::Operation) {
        if (now.content != L"++" && now.content != L"--") return;
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
        get();
        return;
    }

    if (now.content != L"[") return;
    expr_();

    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);

    if (now.content != L"]") return;
    get();
}

void Parser::atom() {
    if (now.type != Lexeme::Identifier && now.type != Lexeme::Literal && now.type != Lexeme::StringLiteral) {
        if (now.content == L"(") expr0_();
        if (now.content != L")") throw bad_lexeme(now, filename_);
    }
    if (now.type == Lexeme::Literal || now.type == Lexeme::StringLiteral) {
        get();
        return;
    }
    get();
    if (now.type == Lexeme::Operation) return;
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    if (now.type != Lexeme::OpenParentheses) return;
    functionCall_();
    get();
}

void Parser::functionCall_() {
    get();
    given_();
    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
}

void Parser::given_() {
    if (now.type == Lexeme::CloseParentheses) return;
    expr_();
    while (now.content == L",") {
        get();
        expr_();
    }
    if (now.type == Lexeme::Other) throw bad_lexeme(now, filename_);
    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now, filename_);
}
