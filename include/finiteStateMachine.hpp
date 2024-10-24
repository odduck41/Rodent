#pragma once
#include <string>
#include <stdexcept>
#include <iostream>
#include <variant>

namespace lexer
{
    namespace States
    {
        struct Begin
        {
        };

        struct RTI
        {
            std::string curr_str = "";
        };

        struct Parentheses
        {
        };

        struct EndOfParentheses
        {
        };

        struct Operation
        {
            std::string curr_str = "";
        };

        struct Literal
        {
            std::string curr_str = "";
        };

        struct String_literal
        {
            std::string curr_str = "";
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
        };

        struct Colon
        {
        };

        struct Punctuation
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
        FSM(const char* text);

        ~FSM() = default;

    private:
        const char* text_;

        void applyState(State state);

        [[nodiscard]]
        static Event getEvent(char);


        // From Begin
        void onEvent(States::Begin& state, Events::Semicolon& event);
        void onEvent(States::Begin& state, Events::CloseCurly& event);
        void onEvent(States::Begin& state, Events::Letter& event);
        void onEvent(States::Begin& state, Events::Number& event);
        void onEvent(States::Begin& state, Events::OpenParentheses& event);

        // From RTI
        void onEvent(States::RTI& state, Events::Dot& event);
        void onEvent(States::RTI& state, Events::Space& event);
        void onEvent(States::RTI& state, Events::Number& event);
        void onEvent(States::RTI& state, Events::Underline& event);
        void onEvent(States::RTI& state, Events::Letter& event);
        void onEvent(States::RTI& state, Events::Colon& event);
        void onEvent(States::RTI& state, Events::OpenCurly& event);
        void onEvent(States::RTI& state, Events::Punctuation& event);
        void onEvent(States::RTI& state, Events::OpenParentheses& event);
        void onEvent(States::RTI& state, Events::Semicolon& event);

        // From Parentheses
        void onEvent(States::Parentheses& state, Events::Letter& event);
        void onEvent(States::Parentheses& state, Events::OpenParentheses& event);
        void onEvent(States::Parentheses& state, Events::CloseParentheses& event);
        void onEvent(States::Parentheses& state, Events::Semicolon& event);

        // From EndOfParentheses
        void onEvent(States::EndOfParentheses& state, Events::CloseParentheses& event);
        void onEvent(States::EndOfParentheses& state, Events::Semicolon& event);

        // From Operation
        void onEvent(States::Operation& state, Events::Letter& event);
        void onEvent(States::Operation& state, Events::Number& event);
        void onEvent(States::Operation& state, Events::Quote& event);
        void onEvent(States::Operation& state, Events::Semicolon& event);

        // From Literal
        void onEvent(States::Literal& state, Events::Number& event);
        void onEvent(States::Literal& state, Events::Dot& event);
        void onEvent(States::Literal& state, Events::Semicolon& event);

        // From String_literal
        void onEvent(States::String_literal& state, Events::Quote& event);
        void onEvent(States::String_literal& state, Event& event);

        // From EndOfStringLiteral
        void onEvent(States::EndOfStringLiteral& state, Events::Punctuation& event);
        void onEvent(States::EndOfStringLiteral& state, Events::Semicolon& event);

        // Default
        void onEvent(State&, Event&);
    };
}
