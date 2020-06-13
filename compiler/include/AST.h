//
// Created by zhaoyiping on 2020/3/31.
//

#ifndef COMPILER_COMPILER_INCLUDE_AST_H_
#define COMPILER_COMPILER_INCLUDE_AST_H_
#include "memory"
#include "vector"
#include "deque"
#include "base.h"
#include "token.h"
#include "symbol.h"
#include "json.hpp"
#include "TAC.h"
using json = nlohmann::json;

#define to_jsonDeclare(type) friend void to_json(json&j,std::shared_ptr<type> p)
// base AST class
class AST {
//  virtual void gen()=0;
 public:
  std::shared_ptr<AST> next;
  virtual json toJSON(bool root = false) = 0;
  virtual address toTAC(TAClist &result,TransInfo transInfo) = 0;
  /*
   * return INT CHAR ARRAY UNDEFINED BOOL
   * may ASTLeaf can return STRING but it should only be used in write
   */
  virtual Tokentype check() = 0;
  AST();
  to_jsonDeclare(AST);
  static std::deque<SymbolTable> symbolTables;
  static std::shared_ptr<Symbol> getSymbol(std::string value);
};
// declare value or function
class ASTDeclare : public AST {
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
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTDeclareFun);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};
// declare value
class ASTDeclareValue : public ASTDeclare {
 public:
  bool isConst;
  Tokentype valueType;
  std::string value;// the value of valueId used in const
  ASTDeclareValue();
  ASTDeclareValue(bool, Tokentype, std::string, std::string);
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTDeclareValue);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};
class ASTDeclareArray : public ASTDeclare {
 public:
  int length;
  Tokentype valueType;
  ASTDeclareArray(int, Tokentype, std::string);
  ASTDeclareArray();
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTDeclareArray);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};
// if else
class ASTCondition : public AST {
 public:
  std::shared_ptr<ASTExpression> cmp;
  std::shared_ptr<AST> thenStatements;
  std::shared_ptr<AST> elseStatements;
  ASTCondition();
  ASTCondition(std::shared_ptr<ASTExpression>, std::shared_ptr<AST>, std::shared_ptr<AST>);
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTCondition);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};
// loop
class ASTLoop : public AST {
 public:
  std::shared_ptr<ASTExpression> cmp;
  std::shared_ptr<AST> body;
  ASTLoop();
  ASTLoop(std::shared_ptr<ASTExpression>, std::shared_ptr<AST>);
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTLoop);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};
// call function
class ASTCall : public AST {
 public:
  std::string funName;
  std::shared_ptr<AST> args;
  ASTCall(std::string, std::shared_ptr<AST>);
  ASTCall();
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTCall);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};
// cmp cal assign and so on
class ASTExpression : public AST {
 public:
  // + - * / && || > < != >= <= ! == = []
  Tokentype operatorType;
  std::shared_ptr<AST> expression1;
  std::shared_ptr<AST> expression2;
  ASTExpression();
  ASTExpression(Tokentype, std::shared_ptr<AST>, std::shared_ptr<AST>);
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTExpression);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};
// ID character integer
class ASTLeaf : public AST {
 public:
  std::string value;
  Tokentype valueType;
  ASTLeaf();
  ASTLeaf(std::string, Tokentype);
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTLeaf);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};

// int
class ASTRead : public AST {
 public:
  std::shared_ptr<ASTLeaf> args;
  ASTRead();
  ASTRead(std::shared_ptr<ASTLeaf> args);
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTRead);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};

// int
class ASTWrite : public AST {
 public:
  std::shared_ptr<AST> args;
  ASTWrite(std::shared_ptr<AST> &&args);
  ASTWrite();
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTWrite);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};

class ASTRet : public AST {
 public:
  std::shared_ptr<AST> value;
  ASTRet();
  ASTRet(std::shared_ptr<AST> value);
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTRet);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};

class ASTSwitch : public AST {
 public:
  std::shared_ptr<AST> expression;
  // default -> ASTLeaf->AST->ASTLeaf->AST...
  std::shared_ptr<AST> cases;
  ASTSwitch(std::shared_ptr<AST> expression, std::shared_ptr<AST> cases);
  ASTSwitch();
  Tokentype check() override;
  json toJSON(bool root = false) override;
  to_jsonDeclare(ASTSwitch);
  address toTAC(TAClist &result,TransInfo transInfo) override;
};

#define toJSONDeclare(type) json toJSON(std::shared_ptr<type>p,bool root=false);

toJSONDeclare(AST)
toJSONDeclare(ASTDeclare)
toJSONDeclare(ASTDeclareFun)
toJSONDeclare(ASTDeclareValue)
toJSONDeclare(ASTDeclareArray)
toJSONDeclare(ASTCondition)
toJSONDeclare(ASTLoop)
toJSONDeclare(ASTCall)
toJSONDeclare(ASTExpression)
toJSONDeclare(ASTLeaf)
toJSONDeclare(ASTRead)
toJSONDeclare(ASTWrite)
toJSONDeclare(ASTRet)
toJSONDeclare(ASTSwitch)

#define toASTDeclare(type) std::shared_ptr<type> type##toAST(const json &j);

toASTDeclare(AST)
toASTDeclare(ASTDeclare)
toASTDeclare(ASTDeclareFun)
toASTDeclare(ASTDeclareValue)
toASTDeclare(ASTDeclareArray)
toASTDeclare(ASTCondition)
toASTDeclare(ASTLoop)
toASTDeclare(ASTCall)
toASTDeclare(ASTExpression)
toASTDeclare(ASTLeaf)
toASTDeclare(ASTRead)
toASTDeclare(ASTWrite)
toASTDeclare(ASTRet)
toASTDeclare(ASTSwitch)

#define toAST(type) type##toAST

#undef toASTDeclare
#undef toJSONDeclare
#undef to_jsonDeclare
#endif //COMPILER_COMPILER_INCLUDE_AST_H_
