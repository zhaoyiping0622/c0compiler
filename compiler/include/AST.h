//
// Created by zhaoyiping on 2020/3/31.
//

#ifndef COMPILER_COMPILER_INCLUDE_AST_H_
#define COMPILER_COMPILER_INCLUDE_AST_H_
#include <vector>
#include "memory"
#include "unordered_map"
#include "vector"
#include "base.h"
#include "token.h"
#include "symbol.h"

// base AST class
class AST {
 public:
  std::shared_ptr<AST> next;
//  virtual void gen()=0;
  AST();
};
// declare value or function
class ASTDeclare : public AST {
//  virtual void install()=0;
};
// declare function
class ASTDeclareFun : public ASTDeclare {
 public:
  Tokentype returnType;
  std::vector<std::pair<Tokentype, std::string>> args;
  std::shared_ptr<AST> body;
  ASTDeclareFun();
  ASTDeclareFun(Tokentype, std::vector<std::pair<Tokentype, std::string>>, std::shared_ptr<AST>);
};
// declare value
class ASTDeclareValue : public ASTDeclare {
 public:
  bool isConst;
  Tokentype valueType;
  Token value;
  ASTDeclareValue();
  ASTDeclareValue(bool, Tokentype, Token);
};
// if else
class ASTCondition : public AST {
 public:
  std::shared_ptr<ASTStatement> cmp;
  std::shared_ptr<AST> thenStatements;
  std::shared_ptr<AST> elseStatements;
  ASTCondition();
  ASTCondition(std::shared_ptr<ASTStatement>, std::shared_ptr<AST>, std::shared_ptr<AST>);
};
// loop
class ASTLoop : public AST {
 public:
  std::shared_ptr<ASTStatement> cmp;
  std::shared_ptr<AST> statements;
  ASTLoop();
  ASTLoop(std::shared_ptr<ASTStatement>, std::shared_ptr<AST>);
};
// call function
class ASTCall : public AST {
 public:
  std::string funName;
  std::vector<std::shared_ptr<AST>> args;
  ASTCall(std::string, std::vector<std::shared_ptr<AST>>);
  ASTCall();
};
// cmp cal assign and so on
class ASTStatement : public AST {
 public:
  Tokentype tokentype;
  std::shared_ptr<AST> statement1;
  std::shared_ptr<AST> statement2;
  ASTStatement();
  ASTStatement(Tokentype, std::shared_ptr<AST>, std::shared_ptr<AST>);
};
// ID character integer
class ASTLeaf : public AST {
 public:
  std::string value;
  Tokentype valuetype;
  ASTLeaf();
  ASTLeaf(std::string, Tokentype);
};
// temporary store ASTs must not be used in the final AST
class ASTVector : public AST {
 public:
  std::vector<std::shared_ptr<AST>> v;
  ASTVector(std::vector<std::shared_ptr<AST>> &&v);
};

#endif //COMPILER_COMPILER_INCLUDE_AST_H_
