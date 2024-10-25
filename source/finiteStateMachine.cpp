#include <finiteStateMachine.hpp>

lexer::FSM::FSM(const char* text, size_t text_length)
    : text_(text),
      text_length_(text_length)
{
    processText();
}

void lexer::FSM::processText()
{
    const char* curr_symbol = text_;
    State curr_state = States::Begin{};

    while (curr_symbol < text_ + text_length_)
    {
        Event event = getEvent(*curr_symbol);
        curr_state = processEvent(curr_state, event);
        ++curr_symbol;
    }
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


lexer::State lexer::FSM::processEvent(State const &state, Event const &event)
{
    State result;
    std::visit(
        overload{
            [this, &result](auto const &state_, auto const &event_)
            {
                result = onEvent(state_, event_);
            }
        },
        state, event
    );

    return result;
}


lexer::State lexer::FSM::onEvent(State const &, Event const &)
{
    throw std::logic_error("Default instatnce of onEvent has been called");
}

lexer::State lexer::FSM::onEvent(States::Begin const &state, Events::Letter const &event)
{
    return States::RTI{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin const &state, Events::Number const &event)
{
    return States::Literal{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin const &state, Events::Semicolon const &event)
{
    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Begin const &state, Events::Colon const &event)
{
    return States::Operation{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin const &state, Events::CloseCurly const &event)
{
    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Begin const &state, Events::OpenParentheses const &event)
{
    return States::Parentheses{};
}


lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::Dot const &event)
{
    lexems_.push_back(applyState(state));

    return States::RTI{};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::Space const &event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::Number const &event)
{
    return States::RTI{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::Underline const &event)
{
    return States::RTI{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::Letter const &event)
{
    return States::RTI{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::Colon const &event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::OpenCurly const &event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::Punctuation const &event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::Operation const &event)
{
    lexems_.push_back(applyState(state));

    return States::Operation{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::OpenParentheses const &event)
{
    lexems_.push_back(applyState(state));

    return States::Parentheses{};
}

lexer::State lexer::FSM::onEvent(States::RTI const &state, Events::Semicolon const &event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::Parentheses const &state, Events::Letter const &event)
{
    lexems_.push_back(applyState(state));

    return States::RTI{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Parentheses const &state, Events::OpenParentheses const &event)
{
    lexems_.push_back(applyState(state));

    return States::Parentheses{};
}

lexer::State lexer::FSM::onEvent(States::Parentheses const &state, Events::CloseParentheses const &event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfParentheses{};
}

lexer::State lexer::FSM::onEvent(States::Parentheses const &state, Events::Semicolon const &event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::EndOfParentheses const &state, Events::CloseParentheses const &event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfParentheses{};
}

lexer::State lexer::FSM::onEvent(States::EndOfParentheses const &state, Events::Semicolon const &event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::Operation const &state, Events::Letter const &event)
{
    lexems_.push_back(applyState(state));

    return States::RTI{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Operation const &state, Events::Number const &event)
{
    lexems_.push_back(applyState(state));

    return States::Literal{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Operation const &state, Events::Quote const &event)
{
    lexems_.push_back(applyState(state));

    return States::String_literal{};
}

lexer::State lexer::FSM::onEvent(States::Operation const &state, Events::Semicolon const &event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::Literal const &state, Events::Number const &event)
{
    return States::Literal{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::Literal const &state, Events::Dot const &event)
{
    return States::Literal{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::Literal const &state, Events::Semicolon const &event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Quote const &event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfStringLiteral{};
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Dot const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Space const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Number const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Underline const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Letter const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Colon const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::OpenCurly const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::CloseCurly const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Punctuation const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Operation const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::OpenParentheses const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::CloseParentheses const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const &state, Events::Semicolon const &event)
{
    return States::String_literal(state.curr_str + event.symbol);
}


lexer::State lexer::FSM::onEvent(States::EndOfStringLiteral const &state, Events::Punctuation const &event)
{
    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::EndOfStringLiteral const &state, Events::Semicolon const &event)
{
    return States::Begin{};
}
