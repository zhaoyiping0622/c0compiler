//
// Created by zhaoyiping on 2020/3/31.
//

#include "token.h"
Token::Token() : tokentype(UNDEFINED), s("") {}
Token::Token(Tokentype tokentype, std::string s) : tokentype(tokentype), s(s) {}
std::string toString(Tokentype tokentype) {
#define TokentypeString(s) case s: return #s;
  switch (tokentype) {
    TokentypeString(ID)
    TokentypeString(ADD)
    TokentypeString(MINUS)
    TokentypeString(MUL)
    TokentypeString(DIV)
    TokentypeString(LT)
    TokentypeString(LE)
    TokentypeString(GT)
    TokentypeString(GE)
    TokentypeString(NE)
    TokentypeString(EQ)
    TokentypeString(NOT)
    TokentypeString(AND)
    TokentypeString(OR)
    TokentypeString(SEMICOLON)
    TokentypeString(LSBRACKETS)
    TokentypeString(RSBRACKETS)
    TokentypeString(LBBRACKETS)
    TokentypeString(RBBRACKETS)
    TokentypeString(LMBRACKETS)
    TokentypeString(RMBRACKETS)
    TokentypeString(ASSIGN)
    TokentypeString(COMMA)
    TokentypeString(COLON)
    TokentypeString(CHARACTER)
    TokentypeString(STRING)
    TokentypeString(UNSIGNED)
    TokentypeString(INT)
    TokentypeString(CHAR)
    TokentypeString(VOID)
    TokentypeString(IF)
    TokentypeString(WHILE)
    TokentypeString(SWITCH)
    TokentypeString(CASE)
    TokentypeString(SCANF)
    TokentypeString(PRINTF)
    TokentypeString(RETURN)
    TokentypeString(MAIN)
    TokentypeString(DEFAULT)
    TokentypeString(CONST)
    TokentypeString(ELSE)
    TokentypeString(EOFTOKEN)
    TokentypeString(UNDEFINED)
    TokentypeString(ARRAY)
    default:std::cerr << tokentype << std::endl;
      error("unexcepted Tokentype");
  }
}
