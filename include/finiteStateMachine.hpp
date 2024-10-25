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
            void operator()() const
            {
            };
        };

        struct RTI
        {
            std::string curr_str;
            void operator()() const
            {
            };
        };

        struct Parentheses
        {
            void operator()() const
            {
            };
        };

        struct EndOfParentheses
        {
            void operator()() const
            {
            };
        };

        struct Operation
        {
            std::string curr_str;
            void operator()() const
            {
            };
        };

        struct Literal
        {
            std::string curr_str;
            void operator()() const
            {
            };
        };

        struct String_literal
        {
            std::string curr_str;
            void operator()() const
            {
            };
        };

        struct EndOfStringLiteral
        {
            void operator()() const
            {
            };
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

    // Я не ебу, что делает этот код
    template <class... Ts>
    struct overload : Ts...
    {
        using Ts::operator()...;
    };

    template <class... Ts>
    overload(Ts...) -> overload<Ts...>;
    // Конец, дальше ебу

    class FSM
    {
    public:
        explicit FSM(const char* text, size_t text_length);

        std::vector<Token> getLexems();

        ~FSM() = default;

    private:
        const char* text_;
        size_t text_length_;
        std::vector<Token> lexems_;

        void processText();


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
        State onEvent(States::Begin const& state, Events::Semicolon const& event);
        State onEvent(States::Begin const& state, Events::Colon const& event);
        State onEvent(States::Begin const& state, Events::OpenCurly const& event);
        State onEvent(States::Begin const& state, Events::CloseCurly const& event);
        State onEvent(States::Begin const& state, Events::Letter const& event);
        State onEvent(States::Begin const& state, Events::Space const& event);
        State onEvent(States::Begin const& state, Events::CloseParentheses const& event);
        State onEvent(States::Begin const& state, Events::Number const& event);
        State onEvent(States::Begin const& state, Events::Operation const& event);
        State onEvent(States::Begin const& state, Events::OpenParentheses const& event);

        // From RTI
        State onEvent(States::RTI const& state, Events::Dot const& event);
        State onEvent(States::RTI const& state, Events::Space const& event);
        State onEvent(States::RTI const& state, Events::Number const& event);
        State onEvent(States::RTI const& state, Events::Underline const& event);
        State onEvent(States::RTI const& state, Events::Letter const& event);
        State onEvent(States::RTI const& state, Events::Colon const& event);
        State onEvent(States::RTI const& state, Events::OpenCurly const& event);
        State onEvent(States::RTI const& state, Events::CloseParentheses const& event);
        State onEvent(States::RTI const& state, Events::Punctuation const& event);
        State onEvent(States::RTI const& state, Events::Operation const& event);
        State onEvent(States::RTI const& state, Events::OpenParentheses const& event);
        State onEvent(States::RTI const& state, Events::Semicolon const& event);

        // From Parentheses
        State onEvent(States::Parentheses const& state, Events::Letter const& event);
        State onEvent(States::Parentheses const& state, Events::OpenParentheses const& event);
        State onEvent(States::Parentheses const& state, Events::CloseParentheses const& event);
        State onEvent(States::Parentheses const& state, Events::Space const& event);
        State onEvent(States::Parentheses const& state, Events::Semicolon const& event);

        // From EndOfParentheses
        State onEvent(States::EndOfParentheses const& state, Events::CloseParentheses const& event);
        State onEvent(States::EndOfParentheses const& state, Events::Semicolon const& event);
        State onEvent(States::EndOfParentheses const& state, Events::Space const& event);

        // From Operation
        State onEvent(States::Operation const& state, Events::Letter const& event);
        State onEvent(States::Operation const& state, Events::Number const& event);
        State onEvent(States::Operation const& state, Events::Quote const& event);
        State onEvent(States::Operation const& state, Events::Space const& event);
        State onEvent(States::Operation const& state, Events::Semicolon const& event);
        State onEvent(States::Operation const& state, Events::Operation const& event);

        // From Literal
        State onEvent(States::Literal const& state, Events::Number const& event);
        State onEvent(States::Literal const& state, Events::Dot const& event);
        State onEvent(States::Literal const& state, Events::Semicolon const& event);

        // From String_literal
        State onEvent(States::String_literal const& state, Events::Quote const& event);
        State onEvent(States::String_literal const& state, Events::Dot const& event);
        State onEvent(States::String_literal const& state, Events::Space const& event);
        State onEvent(States::String_literal const& state, Events::Number const& event);
        State onEvent(States::String_literal const& state, Events::Underline const& event);
        State onEvent(States::String_literal const& state, Events::Letter const& event);
        State onEvent(States::String_literal const& state, Events::Colon const& event);
        State onEvent(States::String_literal const& state, Events::OpenCurly const& event);
        State onEvent(States::String_literal const& state, Events::CloseCurly const& event);
        State onEvent(States::String_literal const& state, Events::Punctuation const& event);
        State onEvent(States::String_literal const& state, Events::Operation const& event);
        State onEvent(States::String_literal const& state, Events::OpenParentheses const& event);
        State onEvent(States::String_literal const& state, Events::CloseParentheses const& event);
        State onEvent(States::String_literal const& state, Events::Semicolon const& event);


        // From EndOfStringLiteral
        State onEvent(States::EndOfStringLiteral const& state, Events::Punctuation const& event);
        State onEvent(States::EndOfStringLiteral const& state, Events::Semicolon const& event);

        // Default
        State onEvent(State const &, Event const &);

        // Process
        State processEvent(State const& state, Event const& event);
    };
}
