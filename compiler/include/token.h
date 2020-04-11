//
// Created by zhaoyiping on 2020/3/31.
//

#ifndef COMPILER_COMPILER_INCLUDE_TOKEN_H_
#define COMPILER_COMPILER_INCLUDE_TOKEN_H_
#include "string"
#include "iostream"
#include "base.h"
#include "symbol.h"
#include "unordered_map"

enum emTokentype : int {
  ID = 257,
  ADD,
  MINUS,
  MUL,
  DIV,
  LT,
  LE,
  GT,
  GE,
  NE,
  EQ,
  NOT,
  AND,
  OR,
  SEMICOLON,
  LSBRACKETS,
  RSBRACKETS,
  LBBRACKETS,
  RBBRACKETS,
  LMBRACKETS,
  RMBRACKETS,
  ASSIGN,
  COMMA,
  COLON,
  CHARACTER,
  STRING,
  UNSIGNED,
  INT,
  CHAR,
  VOID,
  IF,
  WHILE,
  SWITCH,
  CASE,
  SCANF,
  PRINTF,
  RETURN,
  MAIN,
  DEFAULT,
  CONST,
  ELSE,
  EOFTOKEN,
  UNDEFINED,
  ARRAY
};

std::string toString(Tokentype);

class Token {
 public:
  Tokentype tokentype;
  std::string s;
  Token();
  Token(Tokentype tokentype, std::string s);
};

class Tokenizer {
 public:
  virtual Token getToken() = 0;
  std::unordered_map<std::string, Symbol> symbolTable;
};

#endif //COMPILER_COMPILER_INCLUDE_TOKEN_H_
