#pragma once

#include "lexer.hpp"

extern lexer::Trie imported_;

class Parser {
    public:
        explicit Parser(const std::vector<Token>&);
    private:
        void program_();

        void programThings_();
        void import_();

        void functionDefinition_();
        void arguments_();
        void body_();
        void statement_();

        void switch_() {};
        void doWhile_();
        void for_() {};
        void while_();
        void if_();
        void return_();
        void definition_() {};
        void expression_();

        void expr_();
        void expr0_();
        [[nodiscard]] static inline bool op1(const std::wstring&);
        void expr1_();
        void expr2_();
        void expr3_();
        void expr4_();
        void expr5_();
        void expr6_();
        [[nodiscard]] static inline bool op7(const std::wstring&);
        void expr7_();
        void expr8_();
        void expr9_();
        void expr10_();
        [[nodiscard]] static inline bool op11(const std::wstring&);
        void expr11_();
        void expr12_();
        void expr13_();
        void atom();


        void functionCall_();
        void given_();
        void inline_body_();



        bool get();
        std::vector<Token> program;
        Token now;

};

