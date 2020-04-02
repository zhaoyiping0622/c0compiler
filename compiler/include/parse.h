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

void parseError(const char *errorMessage);

class Parse {
 public:
  Parse(std::shared_ptr<Tokenizer> tokenlizer);
 private:
  std::shared_ptr<Tokenizer> tokenlizer;
  std::shared_ptr<AST> root;
  void init();
  void parse();
};

#define ParseStateName(name) ParseState##name
#define ParseStateClassForwardDeclare(name) class ParseStateName(name);
#define ParseStateVectorType std::vector<std::shared_ptr<ParseState>>
#define ParseStateFunctionType std::function<std::shared_ptr<AST>(const std::vector<std::shared_ptr<AST>> &)>
#define RULETYPE std::unordered_map<Tokentype, std::pair<ParseStateVectorType,ParseStateFunctionType>>
#define MakeParseStateVectorElement(name) std::make_shared<ParseStateName(name)>()
#define STATICDECLARE(name) RULETYPE ParseStateName(name)::rules;bool ParseStateName(name)::inited=false;
#define NULLStatement UNDEFINED
class ParseState {
 public:
  static std::shared_ptr<Tokenizer> tokenlizer;
  std::pair<ParseStateVectorType, ParseStateFunctionType > getNext();
 protected:
  static Token popToken();
  static Token getToken();
  static Token nextToken;
  std::shared_ptr<RULETYPE> rules;
};
#define ParseStateClass(name) \
class ParseStateName(name):public ParseState{\
 public:\
  static RULETYPE baseRules;\
  static bool inited;\
  void addRuleVector(Tokentype tokentype,ParseStateVectorType&&v){ rules[tokentype].first=v; }\
  void addRuleFunction(Tokentype tokentype,ParseStateFunctionType&&f){rules[tokentype].second=f;}\
  ParseStateName(name)();\
};

#define ParseStateClassInit(name, ...)\
ParseStateName(name)::ParseStateName(name)(){\
  if(!inited){\
    inited=true;\
    __VA_ARGS__;\
  }\
  rules=std::make_shared(baseRules);\
}

#define ParseStateClassAddVector(tokentype, ...) addRuleVector(tokentype, ParseStateVectorType({__VA_ARGS__}))
#define ParseStateFunctionLambda [](const std::vector<std::shared_ptr<AST>>&)->std::shared_ptr<AST>
#define ParseStateClassAddFunction(tokentype, ...) addRuleFunction(tokentype, ParseStateFunctionLambda{__VA_ARGS__;})
#define ParseStateAST std::vector<std::shared_ptr<AST>>

#endif //COMPILER_COMPILER_INCLUDE_PARSE_H_
