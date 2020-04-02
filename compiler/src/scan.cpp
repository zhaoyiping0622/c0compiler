//
// Created by zhaoyiping on 2020/3/24.
//

#include <memory>
#include "scan.h"
std::string visualize(std::string s) {
  std::string ret;
  for (auto x:s) {
    switch (x) {
      case 0x07: ret += "\\a";
        break;
      case 0x08: ret += "\\b";
        break;
      case 0x09: ret += "\\t";
        break;
      case 0x0a: ret += "\\n";
        break;
      case 0x0b: ret += "\\v";
        break;
      case 0x0c: ret += "\\f";
        break;
      case 0x0d: ret += "\\r";
        break;
      default:ret += x;
    }
  }
  return ret;
}
void scanError(const char *errorMessage) {
  std::string es = "SCAN ERROR: " + (std::string)(errorMessage);
  error(es.c_str());
}
Token Scan::getToken() {
  int state = 1;
  std::string s;
  while (true) {
    char c = getNextChar();
    int nextState = states[state].getNext(c);
    if (nextState != 0) {// after get next char it is a legal state
      state = nextState;
      s += popNextChar();
      continue;
    }
    if (states[state].terminate) {
      if (symbolTable.find(s) != symbolTable.end())
        return Token(symbolTable[s].tokentype, s);
      else if (states[state].tokentype == ID)
        symbolTable[s].tokentype = ID;
      return Token(states[state].tokentype, s);
    } else if (state == 1) {
      popNextChar();
      continue;
    } else {
      s += c;
      s = visualize(s);
      scanError(("\"" + s + "\" is a illegal string").c_str());
    }
  }
}
Scan::Scan(std::shared_ptr<Config> config) : input(config->inputFile) {
  init();
}
Scan::Scan(std::istream &input) : input(input) {
  init();
}
char Scan::getNextChar() {
  char ret;
  if (pointerLocation == bufSize) {
    getNewBuf();
    if (eof)
      return EOF;
  }
  ret = buf[pointerLocation];
  return ret;
}
void Scan::getNewBuf() {
  if (input.eof()) {
    eof = true;
    return;
  }
  input.read(buf, SCANBUFSIZE);
  bufSize = input.gcount();
  pointerLocation = 0;
}
void Scan::init() {
  getNewBuf();
  pointerLocation = 0;
  eof = false;
  initTable();
  initSymbolTable();
}
char Scan::popNextChar() {
  char ret = getNextChar();
  pointerLocation++;
  return ret;
}
void State::setNext(char c, int ne) {
  next[c] = ne;
}
int State::getNext(char c) {
  return next[c];
}
State::State() : terminate(false), tokentype(UNDEFINED) {}
void Scan::initTable() {
#define add(from, to, character) states[from].setNext(character,to)
  states[2].terminate = true;
  states[2].tokentype = AND;
  states[3].terminate = true;
  states[3].tokentype = OR;
  states[4].terminate = true;
  states[4].tokentype = NE;
  states[5].terminate = true;
  states[5].tokentype = NOT;
  states[12].terminate = true;
  states[12].tokentype = CHARACTER;
  states[13].terminate = true;
  states[13].tokentype = LT;
  states[14].terminate = true;
  states[14].tokentype = GT;
  states[15].terminate = true;
  states[15].tokentype = EQ;
  states[16].terminate = true;
  states[16].tokentype = ASSIGN;
  states[17].terminate = true;
  states[17].tokentype = LE;
  states[18].terminate = true;
  states[18].tokentype = GE;
  states[19].terminate = true;
  states[19].tokentype = EOFTOKEN;
  states[20].terminate = true;
  states[20].tokentype = ID;
  states[21].terminate = true;
  states[21].tokentype = ADD;
  states[22].terminate = true;
  states[22].tokentype = MINUS;
  states[23].terminate = true;
  states[23].tokentype = MUL;
  states[24].terminate = true;
  states[24].tokentype = DIV;
  states[25].terminate = true;
  states[25].tokentype = SEMICOLON;
  states[26].terminate = true;
  states[26].tokentype = LSBRACKETS;
  states[27].terminate = true;
  states[27].tokentype = RSBRACKETS;
  states[28].terminate = true;
  states[28].tokentype = LMBRACKETS;
  states[29].terminate = true;
  states[29].tokentype = RMBRACKETS;
  states[30].terminate = true;
  states[30].tokentype = LBBRACKETS;
  states[31].terminate = true;
  states[31].tokentype = RBBRACKETS;
  states[32].terminate = true;
  states[32].tokentype = COMMA;
  states[33].terminate = true;
  states[33].tokentype = COLON;
  states[35].terminate = true;
  states[35].tokentype = STRING;
  states[37].terminate = true;
  states[37].tokentype = UNSIGNED;
  add(1, 5, '!');
  add(5, 4, '=');
  add(1, 6, '&');
  add(6, 2, '&');
  add(1, 7, '|');
  add(7, 3, '|');
  add(1, 8, '\'');
  add(8, 9, '\\');
  for (int i = 0x20; i <= 0x26; i++)add(8, 10, i);
  for (int i = 0x28; i <= 0x5b; i++)add(8, 10, i);
  for (int i = 0x5d; i <= 0x7e; i++)add(8, 10, i);
  for (int i = 0x20; i <= 0x7e; i++)add(9, 11, i);
  add(11, 12, '\'');
  add(10, 12, '\'');
  add(1, 13, '<');
  add(1, 14, '>');
  add(1, 16, '=');
  add(13, 17, '=');
  add(14, 18, '=');
  add(16, 15, '=');
  add(1, 19, EOF);
  add(1, 20, '_');
  for (int i = 'a'; i <= 'z'; i++)add(1, 20, i);
  for (int i = 'A'; i <= 'Z'; i++)add(1, 20, i);
  add(20, 20, '_');
  for (int i = 'a'; i <= 'z'; i++)add(20, 20, i);
  for (int i = 'A'; i <= 'Z'; i++)add(20, 20, i);
  for (int i = '0'; i <= '9'; i++)add(20, 20, i);
  add(1, 21, '+');
  add(1, 22, '-');
  add(1, 23, '*');
  add(1, 24, '/');
  add(1, 25, ';');
  add(1, 26, '(');
  add(1, 27, ')');
  add(1, 28, '[');
  add(1, 29, ']');
  add(1, 30, '{');
  add(1, 31, '}');
  add(1, 32, ',');
  add(1, 33, ':');
  add(1, 34, '"');
  add(34, 34, 0x20);
  add(34, 34, 0x21);
  for (int i = 0x23; i <= 0x5b; i++)add(34, 34, i);
  for (int i = 0x5d; i <= 0x7e; i++)add(34, 34, i);
  add(34, 35, '"');
  add(34, 36, '\\');
  for (int i = 0x20; i <= 0x7e; i++)add(36, 34, i);
  for (int i = '0'; i <= '9'; i++)add(1, 37, i);
  for (int i = '0'; i <= '9'; i++)add(37, 37, i);
#undef add
}
void Scan::initSymbolTable() {
#define add(x, y) symbolTable[#x]=Symbol(y)
  add(int, INT);
  add(char, CHAR);
  add(void, VOID);
  add(if, IF);
  add(while, WHILE);
  add(switch, SWITCH);
  add(case, CASE);
  add(scanf, SCANF);
  add(printf, PRINTF);
  add(return, RETURN);
  add(main, MAIN);
  add(default, DEFAULT);
  add(const, CONST);
  add(else, ELSE);
#undef add
}
