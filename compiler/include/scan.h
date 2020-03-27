//
// Created by zhaoyiping on 2020/3/24.
//

#ifndef COMPILER_COMPILER_INCLUDE_SCAN_H_
#define COMPILER_COMPILER_INCLUDE_SCAN_H_

#include "string"
#include "iostream"
#include "config.h"

#define SCANBUFSIZE 2048
#define STATENUM 33

void scanError(std::string errorMessage);

typedef enum {
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
  SEMICOLON,
  LSBRACKETS,
  RSBRACKETS,
  LBBRACKETS,
  RBBRACKETS,
  LMBRACKETS,
  RMBRACKETS,
  ASSIGN,
  COMMA,
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
  EOFTOKEN,
  UNDEFINED
} Tokentype;

class Token {
 public:
  Tokentype tokentype;
  std::string s;
  Token();
  Token(Tokentype tokentype, std::string s);
};

class State {
 public:
  bool terminate;
  Tokentype tokentype;
  void setNext(char c, int ne);
  int getNext(char c);
  State();
 private:
  std::unordered_map<char, int> next;
};

class Symbol {
 public:
  Tokentype tokentype;
  Symbol(Tokentype tokentype);
  Symbol();
};

class Scan {
 public:
  Token getToken();
  Scan(Config &config);
  Scan(std::istream &input);
 private:
  bool eof;
  std::istream &input;
  char buf[SCANBUFSIZE];
  State states[STATENUM];
  std::unordered_map<std::string, Symbol> symbolTable;
  char nextChar;
  int pointerLocation;
  int bufSize;
  void init();
  void initTable();
  char popNextChar();
  char getNextChar();
  void getNewBuf();
  void initSymbolTable();
};

#endif //COMPILER_COMPILER_INCLUDE_SCAN_H_
