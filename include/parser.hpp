#pragma once

#include "lexer.hpp"

extern lexer::Trie imported_;

class Parser {
    public:
        Parser(const std::vector<Token>&);
    private:
        void program_();

        void programThings_();
        void import_();

        void functionDefinition_();
        void arguments_();
        void body_();
        void statement_();

        void switch_(){};
        void doWhile_(){};
        void for_(){};
        void while_();
        void if_(){};
        void definition_(){};
        void expression(){};

        void functionCall_(){};
        void given_(){};



        bool get();
        std::vector<Token> program;
        Token now;

};

