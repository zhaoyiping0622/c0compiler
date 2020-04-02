//
// Created by zhaoyiping on 2020/3/24.
//

#ifndef COMPILER_COMPILER_INCLUDE_SCAN_H_
#define COMPILER_COMPILER_INCLUDE_SCAN_H_

#include "string"
#include "iostream"
#include "memory"
#include "base.h"
#include "config.h"
#include "symbol.h"
#include "token.h"

#define SCANBUFSIZE 2048
#define STATENUM 38

void scanError(const char *errorMessage);

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
class Scan : public Tokenizer {
 public:
  Token getToken();
  Scan(std::shared_ptr<Config> config);
  Scan(std::istream &input);
 private:
  bool eof;
  std::istream &input;
  char buf[SCANBUFSIZE];
  State states[STATENUM];
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
