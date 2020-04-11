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
#include "json.hpp"
using json=nlohmann::json;

#define to_jsonDeclare(type) friend void to_json(json&j,std::shared_ptr<type> p)
#define to_jsonImplement(type) \
void to_json(json&j,const std::shared_ptr<type> p){\
if(p)j=p->toJSON();\
else j=json(nullptr);\
}

// base AST class
class AST {
//  virtual void gen()=0;
 public:
  std::shared_ptr<AST> next;
  virtual json toJSON() = 0;
  AST();
  to_jsonDeclare(AST);
};
// declare value or function
class ASTDeclare : public AST {
//  virtual void install()=0;
 public:
  std::string valueId;
  to_jsonDeclare(ASTDeclare);
  ASTDeclare();
  ASTDeclare(std::string valueId);
};
// declare function
class ASTDeclareFun : public ASTDeclare {
 public:
  Tokentype returnType;
  std::vector<std::pair<Tokentype, std::string>> args;
  std::shared_ptr<AST> body;
  ASTDeclareFun();
  ASTDeclareFun(Tokentype, std::vector<std::pair<Tokentype, std::string>>, std::shared_ptr<AST>);
  ASTDeclareFun(Tokentype, std::vector<std::pair<Tokentype, std::string>>, std::shared_ptr<AST>, std::string);
  json toJSON() override;
  to_jsonDeclare(ASTDeclareFun);
};
// declare value
class ASTDeclareValue : public ASTDeclare {
 public:
  bool isConst;
  Tokentype valueType;
  std::string value;// the value of valueid used in const
  ASTDeclareValue();
  ASTDeclareValue(bool, Tokentype, std::string, std::string);
  json toJSON() override;
  to_jsonDeclare(ASTDeclareValue);
};
class ASTDeclareArray : public ASTDeclare {
 public:
  int length;
  Tokentype valueType;
  ASTDeclareArray(int, Tokentype, std::string);
  ASTDeclareArray();
  json toJSON() override;
  to_jsonDeclare(ASTDeclareArray);
};
// if else
class ASTCondition : public AST {
 public:
  std::shared_ptr<ASTStatement> cmp;
  std::shared_ptr<AST> thenStatements;
  std::shared_ptr<AST> elseStatements;
  ASTCondition();
  ASTCondition(std::shared_ptr<ASTStatement>, std::shared_ptr<AST>, std::shared_ptr<AST>);
  json toJSON() override;
  to_jsonDeclare(ASTCondition);
};
// loop
class ASTLoop : public AST {
 public:
  std::shared_ptr<ASTStatement> cmp;
  std::shared_ptr<AST> body;
  ASTLoop();
  ASTLoop(std::shared_ptr<ASTStatement>, std::shared_ptr<AST>);
  json toJSON() override;
  to_jsonDeclare(ASTLoop);
};
// call function
class ASTCall : public AST {
 public:
  std::string funName;
  std::shared_ptr<AST> args;
  ASTCall(std::string, std::shared_ptr<AST>);
  ASTCall();
  json toJSON() override;
  to_jsonDeclare(ASTCall);
};
// cmp cal assign and so on
class ASTStatement : public AST {
 public:
  Tokentype operatorType;
  std::shared_ptr<AST> statement1;
  std::shared_ptr<AST> statement2;
  ASTStatement();
  ASTStatement(Tokentype, std::shared_ptr<AST>, std::shared_ptr<AST>);
  json toJSON() override;
  to_jsonDeclare(ASTStatement);
};
// ID character integer
class ASTLeaf : public AST {
 public:
  std::string value;
  Tokentype valueType;
  ASTLeaf();
  ASTLeaf(std::string, Tokentype);
  json toJSON() override;
  to_jsonDeclare(ASTLeaf);
};

// int
class ASTRead : public AST {
 public:
  std::shared_ptr<ASTLeaf> args;
  ASTRead();
  ASTRead(std::shared_ptr<ASTLeaf> args);
  json toJSON() override;
  to_jsonDeclare(ASTRead);
};

// int
class ASTWrite : public AST {
 public:
  std::shared_ptr<AST> args;
  ASTWrite(std::shared_ptr<AST> &&args);
  ASTWrite();
  json toJSON() override;
  to_jsonDeclare(ASTWrite);
};

class ASTRet : public AST {
 public:
  std::shared_ptr<AST> value;
  ASTRet();
  ASTRet(std::shared_ptr<AST> value);
  json toJSON() override;
  to_jsonDeclare(ASTRet);
};

class ASTSwitch : public AST {
 public:
  std::shared_ptr<AST> expression;
  std::shared_ptr<AST> cases;// default -> ASTLeaf->AST->ASTLeaf->AST...
  ASTSwitch(std::shared_ptr<AST> expression, std::shared_ptr<AST> cases);
  ASTSwitch();
  json toJSON() override;
  to_jsonDeclare(ASTSwitch);
};

#undef to_jsonDeclare
#endif //COMPILER_COMPILER_INCLUDE_AST_H_
