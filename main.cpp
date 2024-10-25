#include <lexer.hpp>
#include <tests.h>

#include <vector>

#include "finiteStateMachine.hpp"


int main(int argc, char** argv)
{
    load_reserved();

    std::string code = "func gcd(int a, int b) ->int {\
while (a&&b) {\
if (a<b) {\
    b%=a;\
} else {\
    a%=b;\
}\
}\
return a+b;\
}";

    lexer::FSM fsm(code.c_str(), code.length());
    std::vector <Token> answer = fsm.getLexems();

    for (Token& token : answer)
    {
        std::cout << "---" << '\n';
        std::cout << token.type << "\t\t";
        std::cout << std::string(token.content) << '\n';
    }
    std::cout << "---" << '\n';

    return 0;
}
