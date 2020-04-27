//
// Created by zhaoyiping on 2020/3/31.
//

#ifndef COMPILER_COMPILER_INCLUDE_BASE_H_
#define COMPILER_COMPILER_INCLUDE_BASE_H_
#include "string"

// AST.h
class AST;
class ASTDeclare;
class ASTDeclareFun;
class ASTDeclareValue;
class ASTCondition;
class ASTLoop;
class ASTCall;
class ASTExpression;
class ASTLeaf;
class ASTRead;
class ASTWrite;
class ASTRet;
class ASTSwitch;

// config.h

class Config;

// parse.h

class Parse;
class ParseError;

// scan.h

class State;
class Scan;
class ScanError;

// symbol.h

class Symbol;
class SymbolValue;
class SymbolArray;
class SymbolFunction;
class SymbolTable;


// token.h

class Token;
enum emTokentype : int;
typedef enum emTokentype Tokentype;
class Tokenizer;
std::string toString(Tokentype);

class BaseError : public std::exception {
 public:
  BaseError();
  BaseError(std::string);
  std::string errorMessage;
  void baseOperation();
  virtual ~BaseError();
};

// other function
void error(const char *errorMessage);
int realValue(std::string s);

#endif //COMPILER_COMPILER_INCLUDE_BASE_H_
