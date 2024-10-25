#include <finiteStateMachine.hpp>

lexer::FSM::FSM(const char* text, size_t text_length)
    : text_(text),
      text_length_(text_length)
{
    processText();
    load_reserved();
    load_basic_types();
}

void lexer::FSM::processText()
{
    const char* curr_symbol = text_;
    State curr_state = States::Begin{};

    while (curr_symbol < text_ + text_length_)
    {
        if (*curr_symbol == '\n')
        {
            ++line_;
            ++curr_symbol;
            continue;
        }
        if (*curr_symbol == '\r')
        {
            ++curr_symbol;
            continue;
        }
        Event event = getEvent(*curr_symbol);
        curr_state = processEvent(curr_state, event);
        ++curr_symbol;
    }
}


Token lexer::FSM::applyState(const State& state) const
{
    throw std::logic_error("Default instatnce of applyState has been called");
}

std::vector<Token> lexer::FSM::applyState(const States::Operation& state) const
{
    return operations(state.curr_str, line_);
}

Token lexer::FSM::applyState(const States::Literal& state) const
{
    return {
        Lexeme::literal,
        state.curr_str,
        line_
    };
}

Token lexer::FSM::applyState(const States::String_literal& state) const
{
    return {
        Lexeme::s_literal,
        state.curr_str,
        line_
    };
}

Token lexer::FSM::applyState(const States::RTI& state) const
{
    if (inTrie(state.curr_str.c_str(), reserved_))
    {
        return {
            Lexeme::reserved,
            state.curr_str,
            line_
        };
    }
    else if (inTrie(state.curr_str.c_str(), types_)) /* проверка на тип */
    {
        return {
            Lexeme::type,
            state.curr_str,
            line_
        };
    }
    else
    {
        return {
            Lexeme::identifier,
            state.curr_str,
            line_
        };
    }
}

Token lexer::FSM::applyState(const States::Parentheses& state) const
{
    return {
        Lexeme::parentheses,
        "(",
        line_
    };
}

Token lexer::FSM::applyState(const States::EndOfParentheses& state) const
{
    return {
        Lexeme::parentheses,
        ")",
        line_
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
    if (symbol >= '0' && symbol <= '9') return lexer::Events::Number{symbol};
    if (symbol == '!' || symbol == '%' || symbol == '^' || symbol == '&' || symbol == '*' || symbol == '/' ||
        symbol == '+' || symbol == '-' || symbol == '<' || symbol == '>' || symbol == '=')
        return lexer::Events::Operation{symbol};

    throw std::logic_error("Unknown symbol");
}


std::vector<Token> lexer::FSM::getLexems()
{
    return lexems_;
}


lexer::State lexer::FSM::processEvent(State const& state, Event const& event)
{
    State result;
    std::visit(
        overload{
            [this, &result](auto const& state_, auto const& event_)
            {
                result = onEvent(state_, event_);
            }
        },
        state, event
    );

    return result;
}


lexer::State lexer::FSM::onEvent(State const&, Event const&)
{
    throw std::logic_error("Default instatnce of onEvent has been called");
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::Letter const& event)
{
    return States::RTI{std::string(1, event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::Number const& event)
{
    return States::Literal{std::string(1, event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::Space const& event)
{
    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::Semicolon const& event)
{
    lexems_.push_back(Token{
        punctuation,
        ";",
        line_
    });

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::Colon const& event)
{
    return States::Operation{std::to_string(event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::Operation const& event)
{
    return States::Operation{std::string(1, event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::CloseCurly const& event)
{
    for(Token& token : applyState(States::Operation("}")))
    {
        lexems_.push_back(token);
    }

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::OpenCurly const& event)
{
    for(Token& token : applyState(States::Operation("{")))
    {
        lexems_.push_back(token);
    }

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::OpenParentheses const& event)
{
    return States::Parentheses{};
}

lexer::State lexer::FSM::onEvent(States::Begin const& state, Events::CloseParentheses const& event)
{
    applyState(States::Operation(")"));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::Dot const& event)
{
    lexems_.push_back(applyState(state));

    return States::RTI{};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::Space const& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::Number const& event)
{
    return States::RTI{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::Underline const& event)
{
    return States::RTI{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::CloseParentheses const& event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfParentheses{};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::Letter const& event)
{
    return States::RTI{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::Colon const& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::OpenCurly const& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::Punctuation const& event)
{
    lexems_.push_back(applyState(state));

    return States::Operation{std::string(1, event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::Operation const& event)
{
    lexems_.push_back(applyState(state));

    return States::Operation{std::string(1, event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::OpenParentheses const& event)
{
    lexems_.push_back(applyState(state));

    return States::Parentheses{};
}

lexer::State lexer::FSM::onEvent(States::RTI const& state, Events::Semicolon const& event)
{
    lexems_.push_back(applyState(state));

    lexems_.push_back(Token{
        punctuation,
        ";",
        line_
    });

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::Parentheses const& state, Events::Letter const& event)
{
    lexems_.push_back(applyState(state));

    return States::RTI{std::string(1, event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Parentheses const& state, Events::Space const& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Parentheses const& state, Events::OpenParentheses const& event)
{
    lexems_.push_back(applyState(state));

    return States::Parentheses{};
}

lexer::State lexer::FSM::onEvent(States::Parentheses const& state, Events::CloseParentheses const& event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfParentheses{};
}

lexer::State lexer::FSM::onEvent(States::Parentheses const& state, Events::Semicolon const& event)
{
    lexems_.push_back(applyState(state));

    lexems_.push_back(Token{
        punctuation,
        ";",
        line_
    });

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::EndOfParentheses const& state, Events::CloseParentheses const& event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfParentheses{};
}

lexer::State lexer::FSM::onEvent(States::EndOfParentheses const& state, Events::Semicolon const& event)
{
    lexems_.push_back(applyState(state));

    lexems_.push_back(Token{
        punctuation,
        ";",
        line_
    });

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::EndOfParentheses const& state, Events::Space const& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::Operation const& state, Events::Letter const& event)
{
    for(Token& token : applyState(state))
    {
        lexems_.push_back(token);
    }

    return States::RTI{std::string(1, event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Operation const& state, Events::Number const& event)
{
    for(Token& token : applyState(state))
    {
        lexems_.push_back(token);
    }

    return States::Literal{std::string(1, event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Operation const& state, Events::Quote const& event)
{
    for(Token& token : applyState(state))
    {
        lexems_.push_back(token);
    }

    return States::String_literal{};
}

lexer::State lexer::FSM::onEvent(States::Operation const& state, Events::Semicolon const& event)
{
    for(Token& token : applyState(state))
    {
        lexems_.push_back(token);
    }

    lexems_.push_back(Token{
        punctuation,
        ";",
        line_
    });

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Operation const& state, Events::Space const& event)
{
    for(Token& token : applyState(state))
    {
        lexems_.push_back(token);
    }

    return States::Operation{};
}

lexer::State lexer::FSM::onEvent(States::Operation const& state, Events::Operation const& event)
{
    return States::Operation{state.curr_str + event.symbol};
}


lexer::State lexer::FSM::onEvent(States::Literal const& state, Events::Number const& event)
{
    return States::Literal{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::Literal const& state, Events::Dot const& event)
{
    return States::Literal{state.curr_str + event.symbol};
}

lexer::State lexer::FSM::onEvent(States::Literal const& state, Events::Semicolon const& event)
{
    lexems_.push_back(applyState(state));

    lexems_.push_back(Token{
        punctuation,
        ";",
        line_
    });

    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::Literal const& state, Events::Operation const& event)
{
    lexems_.push_back(applyState(state));

    return States::Operation{std::string(1, event.symbol)};
}

lexer::State lexer::FSM::onEvent(States::Literal const& state, Events::Space const& event)
{
    lexems_.push_back(applyState(state));

    return States::Begin{};
}


lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Quote const& event)
{
    lexems_.push_back(applyState(state));

    return States::EndOfStringLiteral{};
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Dot const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Space const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Number const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Underline const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Letter const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Colon const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::OpenCurly const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::CloseCurly const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Punctuation const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Operation const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::OpenParentheses const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::CloseParentheses const& event)
{
    return States::String_literal(state.curr_str + event.symbol);
}

lexer::State lexer::FSM::onEvent(States::String_literal const& state, Events::Semicolon const& event)
{
    lexems_.push_back(Token{
        punctuation,
        ";",
        line_
    });

    return States::String_literal(state.curr_str + event.symbol);
}


lexer::State lexer::FSM::onEvent(States::EndOfStringLiteral const& state, Events::Punctuation const& event)
{
    return States::Begin{};
}

lexer::State lexer::FSM::onEvent(States::EndOfStringLiteral const& state, Events::Semicolon const& event)
{
    lexems_.push_back(Token{
        punctuation,
        ";",
        line_
    });

    return States::Begin{};
}


std::vector<Token> lexer::operations(const std::string& a, size_t line) {
    std::vector<Token> ret;
    size_t index = 0;
    while (index < a.size()) {
        if (a[index] == '+') {
            if (index + 1 < a.size() &&
                (a[index + 1] == '+' || a[index + 1] == '=')) {
                ret.push_back(Token{
                    operation,
                    a.substr(index, 2),
                    line
                });
                index += 2;
            } else {
                ret.push_back(Token{
                     operation,
                     std::string(1, a[index]),
                     line
                 });
                ++index;
            }
        }
        else if (a[index] == '-') {
            if (index + 1 < a.size() &&
                (a[index + 1] == '-' || a[index + 1] == '=' || a[index + 1] == '>')) {
                    ret.push_back(Token{
                        operation,
                        a.substr(index, 2),
                        line
                    });
                    index += 2;
                } else {
                    ret.push_back(Token{
                         operation,
                         std::string(1, a[index]),
                         line
                     });
                    ++index;
                }
        }
        else if (a[index] == '*' || a[index] == '/' || a[index] == '=' || a[index] == '!') {
            if (index + 1 < a.size() && a[index + 1] == '=') {
                ret.push_back(Token{
                        operation,
                        a.substr(index, 2),
                        line
                    });
                index += 2;
            } else {
                ret.push_back(Token{
                         operation,
                         std::string(1, a[index]),
                         line
                });
                ++index;
            }
        }
        else if (a[index] == '&' || a[index] == '|') {
            if (index + 1 < a.size() &&
                (a[index + 1] == a[index] || a[index + 1] == '=')) {
                ret.push_back(Token{
                        operation,
                        a.substr(index, 2),
                        line
                });
                index += 2;
            } else {
                ret.push_back(Token{
                         operation,
                         std::string(1, a[index]),
                         line
                });
                ++index;
            }
        }
        else if (a[index] == '<' || a[index] == '>') {
            if (index + 1 < a.size() && a[index + 1] == '='
                || a[index + 1] == a[index]) {
                if (index + 2 < a.size() && a[index + 1] == a[index]
                    && a[index + 2] == '=') {
                    ret.push_back(Token{
                        operation,
                        a.substr(index, 3),
                        line
                });
                    index += 3;
                } else {
                    ret.push_back(Token{
                        operation,
                        a.substr(index, 2),
                        line
                });
                    index += 2;
                }
            } else {
                ret.push_back(Token{
                         operation,
                         std::string(1, a[index]),
                         line
                });
                ++index;
            }
        }
        else {
            ret.push_back(Token{
                         operation,
                         std::string(1, a[index]),
                         line
            });
            ++index;
        }
    }
    return ret;
}
