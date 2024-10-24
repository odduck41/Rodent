#pragma once
#include <string>
#include <stdexcept>
#include <iostream>
#include <variant>

namespace lexer {
namespace States {
    struct Begin {};
    
    struct RTI {
        std::string curr_str = "";
    };
    
    struct Parentheses {};
    
    struct EndOfParentheses {};
    
    struct Operation {
        std::string curr_str = "";
    };
    
    struct Literal {
        std::string curr_str = "";
    };
    
    struct String_literal {
        std::string curr_str = "";
    };
    
    struct EndOfStringLiteral {};
}

namespace Events {
    struct Letter {
        char symbol;
    };
        
    struct Space {
        char symbol;
    };
    
    struct Dot {
        char symbol;
    };

    struct OpenParentheses {
        char symbol;
    };
    
    struct CloseParentheses {
        char symbol;
    };
    
    struct Number {
        char symbol;
    };
    
    struct Underline {};

    struct Colon {};
    
    struct Punctuation {
        char symbol;
    };
    
    struct Quote {
        char symbol;
    };
    
    struct OpenCurly {
        char symbol;
    };
    
    struct CloseCurly {
        char symbol;
    };
    
    struct Semicolon {
        char symbol;
    };
    
    struct End {};
}


using State = std::variant <
    lexer::States::Begin,
    lexer::States::RTI,
    lexer::States::Parentheses,
    lexer::States::EndOfParentheses,
    lexer::States::Operation,
    lexer::States::Literal,
    lexer::States::String_literal,
    lexer::States::EndOfStringLiteral
>;


using Event = std::variant <
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
    

class FSM {
  public:
    FSM(const char* text);

    ~FSM() = default;

  private:
    const char* text_;

    // void applyState(State state);

    // [[nodiscard]]
    // static Event getEvent(char);


    // From Begin
    void onEvent(State::Begin s, Event::Semicolon e) {
        std::cout << "yy" << '\n';
    };
    // void onEvent(State::Begin, Event::CloseCurly);
    // void onEvent(State::Begin, Event::Letter);
    // void onEvent(State::Begin, Event::Number);

    // From RTI 
    // void onEvent(States::State::RTI, Events::Event::Number);
    // void onEvent(States::State::RTI, Events::Event::Letter);
    // void onEvent(States::State::RTI, Events::Event::Dot);
    // void onEvent(States::State::RTI, Events::Event::Underline);
    // void onEvent(States::State::RTI, Events::Event::Colon);
    // void onEvent(States::State::RTI, Events::Event::OpenCurly);
    // void onEvent(States::State::RTI, Events::Event::Punctuation);
    // void onEvent(States::State::RTI, Events::Event::OpenParentheses);
    // void onEvent(States::State::RTI, Events::Event::Space);


    void onEvent(State state, Event event) { // Default implementation (throw logic_error)
        throw std::logic_error("You are so stupid asshole");
    }; 
};   
}