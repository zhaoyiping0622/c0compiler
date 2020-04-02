//
// Created by zhaoyiping on 2020/3/31.
//

#ifndef COMPILER_COMPILER_INCLUDE_BASE_H_
#define COMPILER_COMPILER_INCLUDE_BASE_H_

// AST.h
class AST;
class ASTDeclare;
class ASTDeclareFun;
class ASTDeclareValue;
class ASTCondition;
class ASTLoop;
class ASTAssign;
class ASTCall;
class ASTStatement;
class ASTLeaf;

// config.h

class Config;

// parse.h

class Parse;
class ParseState;

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

// other function
void error(const char *errorMessage);

#endif //COMPILER_COMPILER_INCLUDE_BASE_H_
