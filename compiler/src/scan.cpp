//
// Created by zhaoyiping on 2020/3/24.
//

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
void scanError(std::string errorMessage) {
  std::cerr << "SCAN ERROR: " << errorMessage << std::endl;
  exit(1);
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
      scanError("\"" + s + "\" is a illegal string");
    }
  }
}
Scan::Scan(Config &config) : input(config.inputFile) {
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
  nextChar = 0;
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
Token::Token() {

}
Token::Token(Tokentype tokentype, std::string s) : tokentype(tokentype), s(s) {

}
void Scan::initTable() {
#define add(from, to, character) states[from].setNext(character,to)
  states[6].terminate = true;
  states[6].tokentype = CHARACTER;
  states[7].terminate = true;
  states[7].tokentype = LT;
  states[8].terminate = true;
  states[8].tokentype = GT;
  states[10].terminate = true;
  states[10].tokentype = ASSIGN;
  states[11].terminate = true;
  states[11].tokentype = LE;
  states[12].terminate = true;
  states[12].tokentype = GE;
  states[13].terminate = true;
  states[13].tokentype = NE;
  states[14].terminate = true;
  states[14].tokentype = EQ;
  states[15].terminate = true;
  states[15].tokentype = EOFTOKEN;
  states[16].terminate = true;
  states[16].tokentype = ID;
  states[17].terminate = true;
  states[17].tokentype = ADD;
  states[18].terminate = true;
  states[18].tokentype = MINUS;
  states[19].terminate = true;
  states[19].tokentype = MUL;
  states[20].terminate = true;
  states[20].tokentype = DIV;
  states[21].terminate = true;
  states[21].tokentype = SEMICOLON;
  states[22].terminate = true;
  states[22].tokentype = LSBRACKETS;
  states[23].terminate = true;
  states[23].tokentype = RSBRACKETS;
  states[24].terminate = true;
  states[24].tokentype = LMBRACKETS;
  states[25].terminate = true;
  states[25].tokentype = RMBRACKETS;
  states[26].terminate = true;
  states[26].tokentype = LBBRACKETS;
  states[27].terminate = true;
  states[27].tokentype = RBBRACKETS;
  states[28].terminate = true;
  states[28].tokentype = COMMA;
  states[30].terminate = true;
  states[30].tokentype = STRING;
  states[32].terminate = true;
  states[32].tokentype = UNSIGNED;
  add(1, 2, '\'');
  add(2, 3, '\\');
  for (int i = 0x20; i <= 0x26; i++)add(2, 4, i);
  for (int i = 0x28; i <= 0x5b; i++)add(2, 4, i);
  for (int i = 0x5d; i <= 0x7e; i++)add(2, 4, i);
  for (int i = 0x20; i <= 0x7e; i++)add(3, 5, i);
  add(5, 6, '\'');
  add(4, 6, '\'');
  add(1, 7, '<');
  add(1, 8, '>');
  add(1, 9, '!');
  add(1, 10, '=');
  add(7, 11, '=');
  add(8, 12, '=');
  add(9, 13, '=');
  add(10, 14, '=');
  add(1, 15, EOF);
  add(1, 16, '_');
  for (int i = 'a'; i <= 'z'; i++)add(1, 16, i);
  for (int i = 'A'; i <= 'Z'; i++)add(1, 16, i);
  add(16, 16, '_');
  for (int i = 'a'; i <= 'z'; i++)add(16, 16, i);
  for (int i = 'A'; i <= 'Z'; i++)add(16, 16, i);
  for (int i = '0'; i <= '9'; i++)add(16, 16, i);
  add(1, 17, '+');
  add(1, 18, '-');
  add(1, 19, '*');
  add(1, 20, '/');
  add(1, 21, ';');
  add(1, 22, '(');
  add(1, 23, ')');
  add(1, 24, '[');
  add(1, 25, ']');
  add(1, 26, '{');
  add(1, 27, '}');
  add(1, 28, ',');
  add(1, 29, '"');
  add(29, 29, 0x20);
  add(29, 29, 0x21);
  for (int i = 0x23; i <= 0x5b; i++)add(29, 29, i);
  for (int i = 0x5d; i <= 0x7e; i++)add(29, 29, i);
  add(29, 30, '"');
  add(29, 31, '\\');
  for (int i = 0x20; i <= 0x7e; i++)add(31, 29, i);
  for (int i = '0'; i <= '9'; i++)add(1, 32, i);
  for (int i = '0'; i <= '9'; i++)add(32, 32, i);
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
#undef add
}
Symbol::Symbol() {

}
Symbol::Symbol(Tokentype tokentype) : tokentype(tokentype) {

}
