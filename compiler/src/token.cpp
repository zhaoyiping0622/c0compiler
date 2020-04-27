//
// Created by zhaoyiping on 2020/3/31.
//

#include "token.h"
Token::Token() : tokentype(UNDEFINED), s("") {}
Token::Token(Tokentype tokentype, std::string s) : tokentype(tokentype), s(s) {}
std::string toString(Tokentype tokentype) {
#define Token2String(s) case s: return #s;
  switch (tokentype) {
    Token2String(ID)
    Token2String(ADD)
    Token2String(MINUS)
    Token2String(MUL)
    Token2String(DIV)
    Token2String(LT)
    Token2String(LE)
    Token2String(GT)
    Token2String(GE)
    Token2String(NE)
    Token2String(EQ)
    Token2String(NOT)
    Token2String(AND)
    Token2String(OR)
    Token2String(SEMICOLON)
    Token2String(LSBRACKETS)
    Token2String(RSBRACKETS)
    Token2String(LBBRACKETS)
    Token2String(RBBRACKETS)
    Token2String(LMBRACKETS)
    Token2String(RMBRACKETS)
    Token2String(ASSIGN)
    Token2String(COMMA)
    Token2String(COLON)
    Token2String(CHARACTER)
    Token2String(STRING)
    Token2String(UNSIGNED)
    Token2String(INT)
    Token2String(CHAR)
    Token2String(VOID)
    Token2String(IF)
    Token2String(WHILE)
    Token2String(SWITCH)
    Token2String(CASE)
    Token2String(SCANF)
    Token2String(PRINTF)
    Token2String(RETURN)
    Token2String(MAIN)
    Token2String(DEFAULT)
    Token2String(CONST)
    Token2String(ELSE)
    Token2String(EOFTOKEN)
    Token2String(UNDEFINED)
    Token2String(ARRAY)
    default:std::cerr << tokentype << std::endl;
      error("unexcepted Tokentype");
  }
}

Tokentype toTokenType(std::string s) {
  static std::unordered_map<std::string, Tokentype> tokens;
#define String2Token(t) tokens[#t]=t;
  if (tokens.empty()) {
    String2Token(ID)
    String2Token(ADD)
    String2Token(MINUS)
    String2Token(MUL)
    String2Token(DIV)
    String2Token(LT)
    String2Token(LE)
    String2Token(GT)
    String2Token(GE)
    String2Token(NE)
    String2Token(EQ)
    String2Token(NOT)
    String2Token(AND)
    String2Token(OR)
    String2Token(SEMICOLON)
    String2Token(LSBRACKETS)
    String2Token(RSBRACKETS)
    String2Token(LBBRACKETS)
    String2Token(RBBRACKETS)
    String2Token(LMBRACKETS)
    String2Token(RMBRACKETS)
    String2Token(ASSIGN)
    String2Token(COMMA)
    String2Token(COLON)
    String2Token(CHARACTER)
    String2Token(STRING)
    String2Token(UNSIGNED)
    String2Token(INT)
    String2Token(CHAR)
    String2Token(VOID)
    String2Token(IF)
    String2Token(WHILE)
    String2Token(SWITCH)
    String2Token(CASE)
    String2Token(SCANF)
    String2Token(PRINTF)
    String2Token(RETURN)
    String2Token(MAIN)
    String2Token(DEFAULT)
    String2Token(CONST)
    String2Token(ELSE)
    String2Token(EOFTOKEN)
    String2Token(UNDEFINED)
    String2Token(ARRAY)
  }
  auto it = tokens.find(s);
  if (it != tokens.end())
    return it->second;
  else
    return UNDEFINED;
}
