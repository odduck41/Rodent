#pragma once

#include <string>
#include <iostream>
#include <variant>
#include <vector>
#include <lexer.hpp>

namespace lexer
{
    namespace States
    {
        struct Begin
        {
        };

        struct RTI
        {
            std::string curr_str;
        };

        struct Parentheses
        {
        };

        struct EndOfParentheses
        {
        };

        struct Operation
        {
            std::string curr_str;
        };

        struct Literal
        {
            std::string curr_str;
        };

        struct String_literal
        {
            std::string curr_str;
        };

        struct EndOfStringLiteral
        {
        };
    }

    namespace Events
    {
        struct Letter
        {
            char symbol;
        };

        struct Space
        {
            char symbol;
        };

        struct Dot
        {
            char symbol;
        };

        struct OpenParentheses
        {
            char symbol;
        };

        struct CloseParentheses
        {
            char symbol;
        };

        struct Number
        {
            char symbol;
        };

        struct Underline
        {
            char symbol;
        };

        struct Colon
        {
            char symbol;
        };

        struct Punctuation
        {
            char symbol;
        };

        struct Operation
        {
            char symbol;
        };

        struct Quote
        {
            char symbol;
        };

        struct OpenCurly
        {
            char symbol;
        };

        struct CloseCurly
        {
            char symbol;
        };

        struct Semicolon
        {
            char symbol;
        };

        struct End
        {
        };
    }


    using State = std::variant<
        lexer::States::Begin,
        lexer::States::RTI,
        lexer::States::Parentheses,
        lexer::States::EndOfParentheses,
        lexer::States::Operation,
        lexer::States::Literal,
        lexer::States::String_literal,
        lexer::States::EndOfStringLiteral
    >;


    using Event = std::variant<
        lexer::Events::Letter,
        lexer::Events::Space,
        lexer::Events::Dot,
        lexer::Events::OpenParentheses,
        lexer::Events::CloseParentheses,
        lexer::Events::Number,
        lexer::Events::Punctuation,
        lexer::Events::Operation,
        lexer::Events::Quote,
        lexer::Events::OpenCurly,
        lexer::Events::CloseCurly,
        lexer::Events::Semicolon,
        lexer::Events::Colon,
        lexer::Events::Underline
    >;


    class FSM
    {
    public:
        explicit FSM(const char* text);

        std::vector<Token> getLexems();

        ~FSM() = default;
        std::vector<
    private:
        const char* text_;
        std::vector<Token> lexems_;

        [[nodiscard]]
        Token applyState(const States::RTI& state) const;
        [[nodiscard]]
        Token applyState(const States::Operation& state) const;
        [[nodiscard]]
        Token applyState(const States::Literal& state) const;
        [[nodiscard]]
        Token applyState(const States::String_literal& state) const;
        [[nodiscard]]
        Token applyState(const States::Parentheses& state) const;
        [[nodiscard]]
        Token applyState(const States::EndOfParentheses& state) const;
        [[nodiscard]]
        Token applyState(const State& state) const;

        [[nodiscard]]
        static Event getEvent(char symbol);


        // From Begin
        State onEvent(States::Begin& state, Events::Semicolon& event);
        State onEvent(States::Begin& state, Events::Colon& event);
        State onEvent(States::Begin& state, Events::CloseCurly& event);
        State onEvent(States::Begin& state, Events::Letter& event);
        State onEvent(States::Begin& state, Events::Number& event);
        State onEvent(States::Begin& state, Events::OpenParentheses& event);

        // From RTI
        State onEvent(States::RTI& state, Events::Dot& event);
        State onEvent(States::RTI& state, Events::Space& event);
        State onEvent(States::RTI& state, Events::Number& event);
        State onEvent(States::RTI& state, Events::Underline& event);
        State onEvent(States::RTI& state, Events::Letter& event);
        State onEvent(States::RTI& state, Events::Colon& event);
        State onEvent(States::RTI& state, Events::OpenCurly& event);
        State onEvent(States::RTI& state, Events::Punctuation& event);
        State onEvent(States::RTI& state, Events::Operation& event);
        State onEvent(States::RTI& state, Events::OpenParentheses& event);
        State onEvent(States::RTI& state, Events::Semicolon& event);

        // From Parentheses
        State onEvent(States::Parentheses& state, Events::Letter& event);
        State onEvent(States::Parentheses& state, Events::OpenParentheses& event);
        State onEvent(States::Parentheses& state, Events::CloseParentheses& event);
        State onEvent(States::Parentheses& state, Events::Semicolon& event);

        // From EndOfParentheses
        State onEvent(States::EndOfParentheses& state, Events::CloseParentheses& event);
        State onEvent(States::EndOfParentheses& state, Events::Semicolon& event);

        // From Operation
        State onEvent(States::Operation& state, Events::Letter& event);
        State onEvent(States::Operation& state, Events::Number& event);
        State onEvent(States::Operation& state, Events::Quote& event);
        State onEvent(States::Operation& state, Events::Semicolon& event);

        // From Literal
        State onEvent(States::Literal& state, Events::Number& event);
        State onEvent(States::Literal& state, Events::Dot& event);
        State onEvent(States::Literal& state, Events::Semicolon& event);

        // From String_literal
        State onEvent(States::String_literal& state, Events::Quote& event);
        State onEvent(States::String_literal& state, Events::Dot& event);
        State onEvent(States::String_literal& state, Events::Space& event);
        State onEvent(States::String_literal& state, Events::Number& event);
        State onEvent(States::String_literal& state, Events::Underline& event);
        State onEvent(States::String_literal& state, Events::Letter& event);
        State onEvent(States::String_literal& state, Events::Colon& event);
        State onEvent(States::String_literal& state, Events::OpenCurly& event);
        State onEvent(States::String_literal& state, Events::CloseCurly& event);
        State onEvent(States::String_literal& state, Events::Punctuation& event);
        State onEvent(States::String_literal& state, Events::Operation& event);
        State onEvent(States::String_literal& state, Events::OpenParentheses& event);
        State onEvent(States::String_literal& state, Events::CloseParentheses& event);
        State onEvent(States::String_literal& state, Events::Semicolon& event);


        // From EndOfStringLiteral
        State onEvent(States::EndOfStringLiteral& state, Events::Punctuation& event);
        State onEvent(States::EndOfStringLiteral& state, Events::Semicolon& event);

        // Default
        State onEvent(State&, Event&);
    };
}
