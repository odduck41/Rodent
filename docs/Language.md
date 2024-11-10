
# Axioms

+ \<letter> - любая буква любого языка(wchar_t)
+ \<digit> - цифра: \[0, 9]
+ \<symbol> - вообще всё что угодно

# Basic things

+ \<name> ::= \<letter>{\[\<letter> | \<digit>]}
+ \<identifier> ::= \<name>

## Types etc.

+ \<basic_type> ::= int | double | char | bool | str | array 
+ \<user_type> ::=  \<name>// На случай, если мы будем добавлять ООП
+ \<type> ::= \<basic_type> | \<user_type>

## Literals
+ \<int $_{10}$> ::= {\<digit>}
+ \<int $_{2}$> ::= 0b{0 | 1}
+ \<int $_{16}$> ::= 0x{0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F}

+ \<uil> ::= \<int $_{10}$> | \<int $_{2}$> | \<int $_{16}$>
+ \<sign> ::= + | -
+ \<sil> ::= \<sign> \<uil>

+ \<int> ::= \<uil> | \<sil>

+ \<str> ::= "{\<symbol>}"

+ \<literal> ::= \<int> | \<str>

# Expressions

## Operators

+ \<op0> ::= `,`
+ \<op1> ::= `=` | `+=` | `-=` | `*=` | `/=` | `%=` | `&=` | `|=` | `^=` | `<<=` | `>>=`
+ \<op2> ::= `||` | `or`
+ \<op3> ::= `&&` | `and`
+ \<op4> ::= `|`
+ \<op5> ::= `&`
+ \<op6> ::= `==` | `!=`
+ \<op7> ::= `>` | `<` | `<=` | `>=` | `<=>`
+ \<op8> ::= `<<` | `>>`
+ \<op9> ::= `+` | `-`
+ \<op10> ::= `*` | `/` | `%`
+ \<op11> ::= `!` | `++int` | `--int` | `+int` | `-int`
+ \<op12> ::= `int++` | `int--`

## Expressions

+ \<expr0> ::= \<expr1>{\<op0> \<expr1>}
+ \<expr1> ::= \<expr2>{\<op1> \<expr2>}
+ \<expr2> ::= \<expr3>{\<op2> \<expr3>}
+ \<expr3> ::= \<expr4>{\<op3> \<expr4>}
+ \<expr4> ::= \<expr5>{\<op4> \<expr5>}
+ \<expr5> ::= \<expr6>{\<op5> \<expr6>}
+ \<expr6> ::= \<expr7>{\<op6> \<expr7>}
+ \<expr7> ::= \<expr8>{\<op7> \<expr8>}
+ \<expr8> ::= \<expr9>{\<op8> \<expr9>}
+ \<expr9> ::= \<expr10>{\<op9> \<expr10>}
+ \<expr10> ::= \<expr11>{\<op10> \<expr11>}
+ \<expr11> ::= \[\<op11>]\<expr12>
+ \<expr12> ::= \<expr13>\[\<op12>]
+ \<expr13> ::= \<atom>`[`\<expression>`]` | \<atom>`.`\<identifier>
+ \<atom> ::= \<identifier> | \<literal> | \<function_call> | (\<expr0>)

+ \<expr> ::= \<expr0>
+ \<expression> ::= \<expr>`;` | `;`

# Variables

+ \<declaration> ::= extern \<type> \<name> {, \<name>}; // не факт что будет
+ \<def> ::= \<name> = \<literal> | \<name>

+ \<definition> ::= \<type> \<def> {, \<def>};

# Operands

+ \<if> ::= if (\<expr>) `{`{\<statement>}`}` { else \<if> | elif (\<expression>) `{`{\<statement>}`}` } \[else `{`{\<statement>}`}`]

+ \<while> ::= while (\<expr>) `{`{\<loop-statement>}`}`

+ \<for> ::= for (\[\<expr>]; \[\<expr>]; \[\<expr>]) `{`{\<statement>}`}`

+ \<do-while> ::= do `{`{\<statement>}`}` while (\<expr>)

+ \<switch> ::= switch(\<identifier>) `{`{case \<literal> : {\<statement>}} \[default : {\<statement>}]`}`

+ \<statement> ::= \<if> | \<while> | \<for> | \<do-while> | \<switch> | \<return> | \<expression> | `break` | `continue` | \<definition>

# Functions

+ \<return> ::= return \<expression>;

+ \<arguments> ::= \[\<type> \<identifier> {, \<type> \<identifier>} \{, \<type> \<identifier> = \<literal>}]

+ \<function_declaration> ::= func \<identifier> (\<arguments>) -> \<type>;

+ \<body> ::= {\<statement>}
+ \<function_definition> ::= func \<identifier> (\<arguments>) -> \<type> `{`\<body>`}`

+ \<given> ::= \[\<expression>{, \<expression>}]

+ \<function_call> ::= \<identifier>`(`\<given>`)`


## Program

+ \<file> ::= {\<symbol>}.rod
+ \<import> ::= import `"`\<file>`"`


+ \<program_things> ::= \<import> | \<function_declaration> | \<function_definition> | \<declaration> | \<definition>
+ \<enter> ::= func main() -> int `{`\<body>`}`


+ \<program> ::= {\<program_things>}\[enter]{\<program_things>} \<EOF>



