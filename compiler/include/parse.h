//
// Created by zhaoyiping on 2020/3/31.
//

#ifndef COMPILER_COMPILER_INCLUDE_PARSE_H_
#define COMPILER_COMPILER_INCLUDE_PARSE_H_
#include "AST.h"
#include "base.h"
#include "memory"
#include "scan.h"
#include "stack"
#include "token.h"
#include "unordered_map"
#include "vector"
#include "cstdarg"

void parseError(const char *errorMessage);

class Parse {
 public:
  Parse(std::shared_ptr<Tokenizer> tokenlizer);
  Parse();
 protected:
  std::shared_ptr<AST> getTail(std::shared_ptr<AST> now);
  std::shared_ptr<Tokenizer> tokenizer;
  std::shared_ptr<AST> root;
  void init();
  void parse();
  Token getToken();
  Token popToken();
  Token nextToken;
  Token match(Tokentype);
  std::shared_ptr<ASTLeaf> readAddOp(); // DONE
  std::shared_ptr<ASTLeaf> readMulOp(); // DONE
  std::shared_ptr<ASTLeaf> readCmpOp(); // DONE
  std::shared_ptr<AST> readProgram(); // DONE
  std::shared_ptr<ASTDeclareFun> readFunOrMain(); // DONE
  std::shared_ptr<ASTDeclareValue> readConstDeclare(); // DONE
  std::shared_ptr<ASTDeclareValue> readConstDefine(); // DONE
  std::string readInteger(); // DONE
  std::pair<Tokentype, std::string> readDeclareHead(); // DONE
  std::shared_ptr<ASTDeclare> readValueDefine(std::pair<Tokentype, std::string>); // DONE
  std::shared_ptr<ASTLeaf> readConstant(); // DONE
  Tokentype readType(); // DONE
  std::shared_ptr<ASTDeclareFun> readFun(); // DONE
  std::shared_ptr<AST> readCompound_statements(); // DONE
  std::shared_ptr<ASTDeclare> readInFuncValueDeclare(); // DONE
  std::vector<std::pair<Tokentype, std::string>> readArg(); // DONE
  std::shared_ptr<ASTDeclareFun> readMain(); // DONE
  std::shared_ptr<AST> readExpression(); // DONE
  std::shared_ptr<AST> readItem(); // DONE
  std::shared_ptr<AST> readFactor(); // DONE
  std::shared_ptr<AST> readFactor1(); // DONE
  std::shared_ptr<AST> readStatement(); // DONE
  std::shared_ptr<AST> readAssign(std::string); // DONE
  std::shared_ptr<ASTCondition> readCondition(); // DONE
  std::shared_ptr<AST> readElse(); // DONE
  std::shared_ptr<ASTStatement> readBoolean(); // DONE
  std::shared_ptr<ASTStatement> readOr(); // DONE
  std::shared_ptr<ASTStatement> readAnd(); // DONE
  std::shared_ptr<ASTStatement> readNot(); // DONE
  std::shared_ptr<ASTStatement> readCond(); // DONE
  std::shared_ptr<AST> readLoop(); // DONE
  std::shared_ptr<AST> readSwitch(); // DONE
  std::shared_ptr<AST> readSwitch1(); // DONE
  std::shared_ptr<AST> readDefault(); // DONE
  std::shared_ptr<AST> readCases(); // DONE
  std::shared_ptr<AST> readCase(); // DONE
  std::shared_ptr<ASTCall> readCall_fun(std::string id = ""); // DONE
  std::shared_ptr<AST> readArgValue(); // DONE
  std::shared_ptr<AST> readStatements(); // DONE
  std::shared_ptr<ASTRead> readRead();
  std::shared_ptr<ASTWrite> readWrite();
  std::shared_ptr<AST> readWrite2();
  std::shared_ptr<ASTRet> readReturn();
  std::string readCharacter();
  std::string readId();
  std::string readUnsigned();
  std::string readString();
  void parseErrorUnexpectedToken(int num, ...);
  void parseErrorUnexpectedToken(const char *);
};

#endif //COMPILER_COMPILER_INCLUDE_PARSE_H_
