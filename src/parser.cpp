#include "parser.hpp"
#include "common.hpp"
#include "exceptions.hpp"
#include <cstdlib>

bool Parser::get() {
    static size_t now_ = -1;
    ++now_;
    if (now_ >= program.size()) return false;
    now = program[now_];
    return true;
}

Parser::Parser(const std::vector<Token>& t) {
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
            throw bad_lexeme(now);
        }
    } else {
        throw bad_lexeme(now);
    }
}

void Parser::import_() {
    get();
    if (now.type == Lexeme::StringLiteral) {
        if (imported_.check(now.content.c_str())) return;
        imported_.add(now.content.c_str());

        const size_t len = wcstombs(nullptr, now.content.c_str(), 0) + 1;
        const auto buffer = new char[len];

        wcstombs(buffer, now.content.c_str(), len);

        const std::string filename(buffer);

        delete[] buffer;

        doAll(filename);

    } else {
        throw bad_lexeme(now);
    }
}

void Parser::functionDefinition_() {
    get();
    if (now.type != Lexeme::Identifier) throw bad_lexeme(now);

    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now);

    arguments_();

    get();
    if (now.type != Lexeme::Operation || now.content != L"->") throw bad_lexeme(now);

    get();
    if (now.type != Lexeme::Type) throw bad_lexeme(now);

    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now);

    body_();

}

void Parser::arguments_() {
    get();
    while(now.type != Lexeme::CloseParentheses) {
        if (now.type != Lexeme::Type) throw bad_lexeme(now);
        get();
        if (now.type != Lexeme::Identifier) throw bad_lexeme(now);
        get();
        if (now.type == Lexeme::Punctuation && now.content == L",") {
            get();
            continue;
        }
        if (now.content != L",") throw bad_lexeme(now);
        get();
        while(now.type != Lexeme::CloseParentheses) {
            if (now.type != Lexeme::Type) throw bad_lexeme(now);
            get();
            if (now.type != Lexeme::Identifier) throw bad_lexeme(now);
            get();
            if (now.type != Lexeme::Operation && now.content != L"=") throw bad_lexeme(now);
            get();
            if (now.type != Lexeme::Literal && now.type != Lexeme::StringLiteral) throw bad_lexeme(now);

            get();

            if (now.type == Lexeme::Punctuation && now.content == L",") {
                continue;
            }

            throw bad_lexeme(now);
        }
        break;
    }
}

void Parser::body_() {
    get();
    while (now.type != Lexeme::CloseCurly) {
        statement_();
    }
}

void Parser::statement_() {
    get();
    if (now.type == Lexeme::CloseCurly) return;
    if (now.type == Lexeme::Reserved) {
        if (now.content == L"if") if_();
        else if (now.content == L"switch") switch_();
        else if (now.content == L"while") while_();
        else if (now.content == L"do") doWhile_();
        else if (now.content == L"for") for_();
        else throw bad_lexeme(now);
    } else if (now.type == Lexeme::Type) {
        definition_();
    } else if (now.type != Lexeme::Other) {
        expression();
    } else throw bad_lexeme(now);
}

void Parser::while_() {
    get();
    if (now.type != Lexeme::OpenParentheses) throw bad_lexeme(now);

    expression();

    if (now.type != Lexeme::CloseParentheses) throw bad_lexeme(now);

    get();
    if (now.type != Lexeme::OpenCurly) throw bad_lexeme(now);

    get();
    while (now.type != Lexeme::CloseCurly) {
        statement_();
    }
}



