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
class ASTVector;
class ASTRead;
class ASTWrite;
class ASTRet;
class ASTSwitch;

// config.h

class Config;

// parse.h

class Parse;

// scan.h

class State;
class Scan;

// symbol.h

class Symbol;

// token.h

class Token;
enum emTokentype : int;
typedef enum emTokentype Tokentype;
class Tokenizer;
std::string toString(Tokentype);

// other function
void error(const char *errorMessage);

#endif //COMPILER_COMPILER_INCLUDE_BASE_H_
