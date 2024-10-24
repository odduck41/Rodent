#include <finiteStateMachine.hpp>

lexer::FSM::FSM(const char* text)
    : text_(text)
{
}


Token lexer::FSM::applyState(const State& state) const
{
    throw std::logic_error("Default instatnce of applyState has been called");
}

Token lexer::FSM::applyState(const States::Operation& state) const
{
    return {
        Lexeme::operation,
        state.curr_str.c_str(),
        0
    };
}

Token lexer::FSM::applyState(const States::Literal& state) const
{
    return {
        Lexeme::literal,
        state.curr_str.c_str(),
        0
    };
}

Token lexer::FSM::applyState(const States::String_literal& state) const
{
    return {
        Lexeme::s_literal,
        state.curr_str.c_str(),
        0
    };
}

Token lexer::FSM::applyState(const States::RTI& state) const
{
    if (inTrie(state.curr_str.c_str()))
    {
        return {
            Lexeme::s_literal,
            state.curr_str.c_str(),
            0
        };
    }
    else if (rand() / 2) /* проверка на тип */
    {
        return {
            Lexeme::type,
            state.curr_str.c_str(),
            0
        };
    }
    else
    {
        return {
            Lexeme::identifier,
            state.curr_str.c_str(),
            0
        };
    }
}

Token lexer::FSM::applyState(const States::Parentheses& state) const
{
    return {
        Lexeme::s_literal,
        "{",
        0
    };
}

Token lexer::FSM::applyState(const States::EndOfParentheses& state) const
{
    return {
        Lexeme::s_literal,
        "}",
        0
    };
}


lexer::Event lexer::FSM::getEvent(const char symbol)
{
    if (symbol == '_') return lexer::Events::Underline{symbol};
    if (symbol == ':') return lexer::Events::Colon{symbol};
    if (symbol == ';') return lexer::Events::Semicolon{symbol};
    if (symbol == '{') return lexer::Events::OpenCurly{symbol};
    if (symbol == '}') return lexer::Events::CloseCurly{symbol};
    if (symbol == '(') return lexer::Events::OpenParentheses{symbol};
    if (symbol == ')') return lexer::Events::CloseParentheses{symbol};
    if (symbol == '.') return lexer::Events::Dot{symbol};
    if (symbol == ' ') return lexer::Events::Space{symbol};
    if (symbol == ',') return lexer::Events::Punctuation{symbol};
    if (symbol == '"' || symbol == '\'') return lexer::Events::Quote{symbol};
    if (symbol >= 'a' && symbol <= 'z') return lexer::Events::Letter{symbol};
    if (symbol >= 'A' && symbol <= 'Z') return lexer::Events::Letter{symbol};
    if (symbol >= '0' && symbol <= '1') return lexer::Events::Number{symbol};
    if (symbol == '!' || symbol == '%' || symbol == '^' || symbol == '&' || symbol == '*' || symbol == '/' ||
        symbol == '+' || symbol == '-' || symbol == '<' || symbol == '>' || symbol == '=')
        return lexer::Events::Operation{symbol};

    throw std::logic_error("Unknown symbol");
}


std::vector<Token> lexer::FSM::getLexems()
{
    return lexems_;
}


lexer::State lexer::FSM::onEvent(State&, Event&)
{
    throw std::logic_error("Default instatnce of onEvent has been called");
}

lexer::State lexer::FSM::onEvent(States::Begin& state, Events::Letter& event)
{
    return States::RTI{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin& state, Events::Number& event)
{
    return States::Literal{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin& state, Events::Semicolon& event)
{
    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Begin& state, Events::Colon& event)
{
    return States::Operation{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin& state, Events::CloseCurly& event)
{
    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Begin& state, Events::OpenParentheses& event)
{
    return States::Parentheses{};
}


lexer::State lexer::FSM::onEvent(States::RTI& state, Events::Dot& event)
{
    lexems_.push_back(applyState(state));

    return States::RTI{};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::Space& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::Number& event)
{
    return States::RTI{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::Underline& event)
{
    return States::RTI{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::Letter& event)
{
    return States::RTI{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::Colon& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::OpenCurly& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::Punctuation& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::Operation& event)
{
    lexems_.push_back(applyState(state));

    return States::Operation{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::OpenParentheses& event)
{
    lexems_.push_back(applyState(state));

    return States::Parentheses{};
}

lexer::State lexer::FSM::onEvent(States::RTI& state, Events::Semicolon& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::Parentheses& state, Events::Letter& event)
{
    lexems_.push_back(applyState(state));

    return States::RTI{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Parentheses& state, Events::OpenParentheses& event)
{
    lexems_.push_back(applyState(state));

    return States::Parentheses{};
}

lexer::State lexer::FSM::onEvent(States::Parentheses& state, Events::CloseParentheses& event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfParentheses{};
}

lexer::State lexer::FSM::onEvent(States::Parentheses& state, Events::Semicolon& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::EndOfParentheses& state, Events::CloseParentheses& event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfParentheses{};
}

lexer::State lexer::FSM::onEvent(States::EndOfParentheses& state, Events::Semicolon& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::Operation& state, Events::Letter& event)
{
    lexems_.push_back(applyState(state));

    return States::RTI{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Operation& state, Events::Number& event)
{
    lexems_.push_back(applyState(state));

    return States::Literal{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Operation& state, Events::Quote& event)
{
    lexems_.push_back(applyState(state));

    return States::String_literal{};
}

lexer::State lexer::FSM::onEvent(States::Operation& state, Events::Semicolon& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::Literal& state, Events::Number& event)
{
    return States::Literal{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::Literal& state, Events::Dot& event)
{
    return States::Literal{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::Literal& state, Events::Semicolon& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Quote& event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfStringLiteral{};
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Dot& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Space& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Number& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Underline& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Letter& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Colon& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::OpenCurly& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::CloseCurly& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Punctuation& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Operation& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::OpenParentheses& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::CloseParentheses& event) {
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal& state, Events::Semicolon& event) {
    return States::String_literal(state.curr_str + event.symbol);
}


lexer::State lexer::FSM::onEvent(States::EndOfStringLiteral& state, Events::Punctuation& event)
{
    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::EndOfStringLiteral& state, Events::Semicolon& event)
{
    return States::Begin{};
}
