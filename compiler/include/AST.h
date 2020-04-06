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
//  virtual void gen()=0;
 public:
  std::shared_ptr<AST> next;
  AST();
};
// declare value or function
class ASTDeclare : public AST {
//  virtual void install()=0;
 public:
  std::string valueId;
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
  std::string value;// the value of valueid used in const
  ASTDeclareValue();
  ASTDeclareValue(bool, Tokentype, std::string, std::string);
};
class ASTDeclareArray : public ASTDeclare {
 public:
  int length;
  Tokentype valueType;
  ASTDeclareArray(int, Tokentype, std::string);
  ASTDeclareArray();
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
  std::shared_ptr<AST> args;
  ASTCall(std::string, std::shared_ptr<AST>);
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
  ASTVector();
  ASTVector(std::vector<std::shared_ptr<AST>> &&v);
};

// int
class ASTRead : public AST {
 public:
  std::shared_ptr<ASTLeaf> args;
  ASTRead();
  ASTRead(std::shared_ptr<ASTLeaf> args);
};

// int
class ASTWrite : public AST {
 public:
  std::shared_ptr<AST> args;
  ASTWrite(std::shared_ptr<AST> &&args);
  ASTWrite();
};

class ASTRet : public AST {
 public:
  std::shared_ptr<AST> value;
  ASTRet();
  ASTRet(std::shared_ptr<AST> value);
};

class ASTSwitch : public AST {
 public:
  std::shared_ptr<ASTStatement> statement;
  std::shared_ptr<AST> cases;// default -> ASTLeaf->AST->ASTLeaf->AST...
  ASTSwitch(std::shared_ptr<ASTStatement> statement, std::shared_ptr<AST> cases);
  ASTSwitch();
};

#endif //COMPILER_COMPILER_INCLUDE_AST_H_
