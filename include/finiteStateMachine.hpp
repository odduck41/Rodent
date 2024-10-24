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
    void onEvent(States::Begin s, Events::Semicolon e) {
        std::cout << "yy" << '\n';
    };
    void onEvent(States::Begin, Events::CloseCurly);
    void onEvent(States::Begin, Events::Letter);
    void onEvent(States::Begin, Events::Number);

    // From RTI 
    void onEvent(States::RTI, Events::Number);
    void onEvent(States::RTI, Events::Letter);
    void onEvent(States::RTI, Events::Dot);
    void onEvent(States::RTI, Events::Underline);
    void onEvent(States::RTI, Events::Colon);
    void onEvent(States::RTI, Events::OpenCurly);
    void onEvent(States::RTI, Events::Punctuation);
    void onEvent(States::RTI, Events::OpenParentheses);
    void onEvent(States::RTI, Events::Space);


    void onEvent(State state, Event event) { // Default implementation (throw logic_error)
        throw std::logic_error("You are so stupid asshole");
    }; 
};   
}