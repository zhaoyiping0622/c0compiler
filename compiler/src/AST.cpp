//
// Created by zhaoyiping on 2020/3/31.
//

#include "AST.h"
#include "base.h"
#include "parse.h"
#include "functional"
#include "unordered_map"
#include "algorithm"

AST::AST() : next(nullptr) {}
ASTDeclareFun::ASTDeclareFun() {}
ASTDeclareFun::ASTDeclareFun(Tokentype returnType,
                             std::vector<std::pair<Tokentype, std::string>> args,
                             std::shared_ptr<AST> body) : returnType(returnType), args(args), body(body) {}
ASTDeclareFun::ASTDeclareFun(Tokentype returnType,
                             std::vector<std::pair<Tokentype, std::string>> args,
                             std::shared_ptr<AST> body,
                             std::string valueId)
    : ASTDeclare(valueId), returnType(returnType), args(args), body(body) {}
ASTDeclareValue::ASTDeclareValue() {}
ASTDeclareValue::ASTDeclareValue(bool isConst, Tokentype valueType, std::string valueId, std::string value)
    : ASTDeclare(valueId), isConst(isConst), valueType(valueType), value(value) {}
ASTCondition::ASTCondition() {}
ASTCondition::ASTCondition(std::shared_ptr<ASTExpression> cmp,
                           std::shared_ptr<AST> thenStatements,
                           std::shared_ptr<AST> elseStatements)
    : cmp(cmp), thenStatements(thenStatements), elseStatements(elseStatements) {}
ASTLoop::ASTLoop() {}
ASTLoop::ASTLoop(std::shared_ptr<ASTExpression> cmp, std::shared_ptr<AST> statements)
    : cmp(cmp), body(statements) {}
ASTCall::ASTCall() {}
ASTCall::ASTCall(std::string funName, std::shared_ptr<AST> args)
    : funName(funName), args(args) {}
ASTExpression::ASTExpression() {}
ASTExpression::ASTExpression(Tokentype operatorType,
                             std::shared_ptr<AST> expression1,
                             std::shared_ptr<AST> expression2)
    : operatorType(operatorType), expression1(expression1), expression2(expression2) {}
ASTLeaf::ASTLeaf() {}
ASTLeaf::ASTLeaf(std::string value, Tokentype tokentype) : value(value), valueType(tokentype) {}
ASTRead::ASTRead() {}
ASTRead::ASTRead(std::shared_ptr<ASTLeaf> args) : args(args) {}
ASTWrite::ASTWrite() {}
ASTWrite::ASTWrite(std::shared_ptr<AST> &&args) : args(args) {}
ASTRet::ASTRet() {}
ASTRet::ASTRet(std::shared_ptr<AST> value) : value(value) {}
ASTSwitch::ASTSwitch() {}
ASTSwitch::ASTSwitch(std::shared_ptr<AST> statement, std::shared_ptr<AST> cases)
    : expression(statement), cases(cases) {}
ASTDeclareArray::ASTDeclareArray() {}
ASTDeclareArray::ASTDeclareArray(int length,
                                 Tokentype valuetype,
                                 std::string valueId)
    : ASTDeclare(valueId), length(length), valueType(valuetype) {}
ASTDeclare::ASTDeclare() {}
ASTDeclare::ASTDeclare(std::string valueId) : valueId(valueId) {}

#define toJSONImplement(type) json toJSON(std::shared_ptr<type>p,bool root){\
  if(p)return p->toJSON(root);\
  return nullptr;\
}

#define to_jsonImplement(type) \
void to_json(json&j,const std::shared_ptr<type> p){\
  if(p)j=p->toJSON();\
  else j=json(nullptr);\
}

#define makeArray \
if (root) {\
  json reallyRet;\
  reallyRet[0] = ret;\
  auto ne = next;\
  int cnt = 1;\
  while(ne) {\
    reallyRet[cnt++] = ne;\
    ne = ne->next;\
  }\
  ret = reallyRet;\
}

to_jsonImplement(AST)
to_jsonImplement(ASTDeclare)
to_jsonImplement(ASTDeclareFun)
to_jsonImplement(ASTDeclareValue)
to_jsonImplement(ASTDeclareArray)
to_jsonImplement(ASTCondition)
to_jsonImplement(ASTLoop)
to_jsonImplement(ASTCall)
to_jsonImplement(ASTExpression)
to_jsonImplement(ASTLeaf)
to_jsonImplement(ASTRead)
to_jsonImplement(ASTWrite)
to_jsonImplement(ASTRet)
to_jsonImplement(ASTSwitch)

toJSONImplement(AST)
toJSONImplement(ASTDeclare)
toJSONImplement(ASTDeclareFun)
toJSONImplement(ASTDeclareValue)
toJSONImplement(ASTDeclareArray)
toJSONImplement(ASTCondition)
toJSONImplement(ASTLoop)
toJSONImplement(ASTCall)
toJSONImplement(ASTExpression)
toJSONImplement(ASTLeaf)
toJSONImplement(ASTRead)
toJSONImplement(ASTWrite)
toJSONImplement(ASTRet)
toJSONImplement(ASTSwitch)

json ASTDeclareFun::toJSON(bool root) {
  json ret;
  ret["name"] = "function declare";
  ret["returnType"] = toString(returnType);
  if (args.size())
    for (int i = 0; i < args.size(); i++)
      ret["args"][i] =
          {
              {"valueType", toString(args[i].first)},
              {"valueId", args[i].second}
          };
  else
    ret["args"] = nullptr;
  ret["body"] = ::toJSON(body, true);
  ret["valueId"] = valueId;
  makeArray;
  return ret;
}
json ASTDeclareValue::toJSON(bool root) {
  json ret;
  ret["name"] = "value declare";
  ret["valueId"] = valueId;
  ret["const"] = isConst;
  ret["valueType"] = toString(valueType);
  ret["value"] = value;
  makeArray;
  return ret;
}
json ASTDeclareArray::toJSON(bool root) {
  json ret;
  ret["name"] = "array declare";
  ret["valueId"] = valueId;
  ret["length"] = length;
  ret["valueType"] = toString(valueType);
  makeArray;
  return ret;
}
json ASTCondition::toJSON(bool root) {
  json ret;
  ret["name"] = "condition";
  ret["cmp"] = cmp;
  ret["then"] = ::toJSON(thenStatements, true);
  ret["else"] = ::toJSON(elseStatements, true);
  makeArray;
  return ret;
}
json ASTLoop::toJSON(bool root) {
  json ret;
  ret["name"] = "loop";
  ret["cmp"] = cmp;
  ret["body"] = ::toJSON(body, true);
  makeArray;
  return ret;
}
json ASTCall::toJSON(bool root) {
  json ret;
  ret["name"] = "call";
  ret["funName"] = funName;
  ret["args"] = ::toJSON(args, true);
  makeArray;
  return ret;
}
json ASTExpression::toJSON(bool root) {
  json ret;
  ret["name"] = "expression";
  ret["operatorType"] = toString(operatorType);
  ret["expression1"] = expression1;
  ret["expression2"] = expression2;
  makeArray;
  return ret;
}
json ASTLeaf::toJSON(bool root) {
  json ret;
  ret["name"] = "leaf";
  ret["value"] = value;
  ret["valueType"] = toString(valueType);
  makeArray;
  return ret;
}
json ASTRead::toJSON(bool root) {
  json ret;
  ret["name"] = "read";
  ret["args"] = ::toJSON(args, true);
  makeArray;
  return ret;
}
json ASTWrite::toJSON(bool root) {
  json ret;
  ret["name"] = "write";
  ret["args"] = ::toJSON(args, true);
  makeArray;
  return ret;
}
json ASTRet::toJSON(bool root) {
  json ret;
  ret["name"] = "ret";
  ret["value"] = value;
  makeArray;
  return ret;
}
json ASTSwitch::toJSON(bool root) {
  json ret;
  ret["name"] = "switch";
  ret["cases"] = ::toJSON(cases, true);
  ret["expression"] = expression;
  makeArray;
  return ret;
}

std::deque<SymbolTable> AST::symbolTables = std::deque<SymbolTable>(1, SymbolTable());
std::shared_ptr<Symbol> AST::getSymbol(std::string value) {
  auto symbol = symbolTables.back().get(value);
  if (symbol)
    return symbol;
  symbol = symbolTables[0].get(value);
  return symbol;
}

int ASTListOperation(std::shared_ptr<AST> ast,
                     std::function<void(std::shared_ptr<AST>)> f = [](std::shared_ptr<AST> ast) { ast->check(); }) {
  int cnt = 0;
  auto now = ast;
  while (now) {
    f(now);
    now = now->next;
    cnt++;
  }
  return cnt;
}

Tokentype ASTDeclareFun::check() {
  auto &symbolTable = symbolTables.back();
  auto ptr = symbolTable.insert<SymbolFunction>(valueId);
  if (!ptr) {
    parseErrorRedefinition(valueId + " is declared as global value");
  } else {
    ptr->returnType = returnType;
    auto funSymbolTable = SymbolTable(valueId);
    for (auto &[argType, argName]:args) {
      ptr->argType.emplace_back(argType);
      auto localPtr = funSymbolTable.insert<SymbolValue>(argName, argType);
      if (!localPtr) {
        parseErrorRedefinition(valueId + " has been declared");
      }
    }
    symbolTables.emplace_back(funSymbolTable);
    ASTListOperation(body, [](std::shared_ptr<AST> ast) {
      auto toASTDeclareFun = std::dynamic_pointer_cast<ASTDeclareFun>(ast);
      if (toASTDeclareFun) {
        parseErrorRedefinition(toASTDeclareFun->valueId + " is declared in function");
      } else {
        ast->check();
      }
    });
    ptr->funSymbolTable = std::make_shared<SymbolTable>(symbolTables.back());
    symbolTables.pop_back();
  }
  return UNDEFINED;
}
Tokentype ASTDeclareValue::check() {
  auto &symbolTable = symbolTables.back();
  if (!symbolTable.insert<SymbolValue>(valueId, valueType, isConst, realValue(value))) {
    parseErrorRedefinition(valueId + " has been declared");
  }
  return UNDEFINED;
}
Tokentype ASTDeclareArray::check() {
  auto &symbolTable = symbolTables.back();
  if (!symbolTable.insert<SymbolArray>(valueId, valueType, length)) {
    parseErrorRedefinition(valueId + " has been declared");
  }
  return UNDEFINED;
}
Tokentype ASTCondition::check() {
  if (cmp->check() != BOOL)
    parseError("the compare expression in if must be bool");
  ASTListOperation(thenStatements);
  ASTListOperation(elseStatements);
  return UNDEFINED;
}
Tokentype ASTLoop::check() {
  if (cmp->check() != BOOL)
    parseError("the compare expression in while must be bool");
  ASTListOperation(body);
  return UNDEFINED;
}
Tokentype ASTCall::check() {
  auto symbol = symbolTables[0].get(funName);
  if (!symbol || symbol->symbolType != FUNCTION) {
    parseError("function " + funName + " is not declared");
  } else {
    // char can be changed to int
    // so the type of argument doesn't care
    int argCnt = ASTListOperation(args);
    auto symbolFunction = std::static_pointer_cast<SymbolFunction>(symbol);
    if (argCnt != symbolFunction->argType.size()) {
      parseError("function " + funName + " needs " + std::to_string(symbolFunction->argType.size())
                     + " arguments but the code provides " + std::to_string(argCnt));
    }
    if (symbolFunction->returnType == INT || symbolFunction->returnType == CHAR)
      return symbolFunction->returnType;
  }
  return UNDEFINED;
}
Tokentype ASTExpression::check() {
  if (operatorType == UNDEFINED)return UNDEFINED;
  Tokentype type1 = expression1->check();
  if (type1 == STRING)
    parseError("operations on string has not been supported");
  if (!expression2 || expression2->check() == UNDEFINED) {
    // - !
    switch (operatorType) {
      case MINUS:
        if (type1 == UNDEFINED || type1 == BOOL) {
          parseError("'-' cannot be followed by error type");
        } else {
          return INT;
        }
      case NOT:
        if (type1 == UNDEFINED) {
          parseError("'!' cannot be followed by error type");
        } else {
          return BOOL;
        }
      default:parseError(toString(operatorType) + " must have more than 1 argument");
    }
  } else {
    Tokentype type2 = expression2->check();
    if (type2 == STRING)
      parseError("operations on string has not been supported");
    if (type1 == UNDEFINED || type2 == UNDEFINED) {
      parseError("argument of " + toString(operatorType) +
          " should not be UNDEFINED");
    }
    switch (operatorType) {
      // + - * /
      case ADD:
      case MINUS:
      case MUL:
      case DIV:
        if (type1 == BOOL || type2 == BOOL) {
          parseError("argument of " + toString(operatorType) + " should not be BOOL");
        } else {
          return INT;
        }
        // && || > < != >= <= ==
      case AND:
      case OR:
      case LT:
      case GT:
      case NE:
      case GE:
      case LE:
      case EQ:return BOOL;
        // =
      case ASSIGN: {
        auto astLeaf = std::dynamic_pointer_cast<ASTLeaf>(expression1);
        auto astExpression = std::dynamic_pointer_cast<ASTExpression>(expression1);
        if (astLeaf && astLeaf->valueType == ID) {
          auto symbol = std::dynamic_pointer_cast<SymbolValue>(getSymbol(astLeaf->value));
          if (!symbol || symbol->isConst) {
            parseError(astLeaf->value + " is a const value");
          }
        } else if (!astExpression || astExpression->operatorType != ARRAY)
          parseError("only variable or array can be left value");
      }
        if (type2 == UNDEFINED || type2 == BOOL)
          parseError("only INT CHAR ARRAY can be right value");
        return type1;
        // []
      case ARRAY: {
        auto astLeaf = std::dynamic_pointer_cast<ASTLeaf>(expression1);
        if (astLeaf) {
          auto symbol = getSymbol(astLeaf->value);
          if (symbol->symbolType != ARRAY)
            parseError(astLeaf->value + " is not an array");
          auto symbolArray = std::dynamic_pointer_cast<SymbolArray>(symbol);
          type1 = symbolArray->valueType;
        } else {
          parseError("only array can have index");
        }
        if (type2 == UNDEFINED || type2 == BOOL || type2 == ARRAY)
          parseError("array index must be int or char");
        return type1;
      }
    }
  }
}
Tokentype ASTLeaf::check() {
  if (valueType != ID) {
    switch (valueType) {
      case INT:
      case UNSIGNED:return INT;
      case CHAR:
      case CHARACTER:return CHAR;
      case UNDEFINED:return UNDEFINED;
      case STRING:return STRING;
      default:
        // TODO: delete this after test
        std::cerr << toString(valueType) << std::endl;
        return UNDEFINED;
    }
  }
  auto symbol = getSymbol(value);
  if (!symbol) {
    parseError(value + " has not been declared");
  }
  if (symbol->symbolType == FUNCTION) {
    parseError(value + " is a function");
  }
  if (symbol->symbolType == ARRAY)return ARRAY;
  return std::dynamic_pointer_cast<SymbolValue>(symbol)->valueType;
}
Tokentype ASTRead::check() {
  ASTListOperation(args, [&](std::shared_ptr<AST> ast) {
    ast->check();
    /*
     * parse process ensures that args of read are ID
     * not array
     */
    auto astLeaf = std::static_pointer_cast<ASTLeaf>(ast);
    if (astLeaf->valueType != ID) {
      parseError("you can only read a variable");
    }
    auto symbol = getSymbol(astLeaf->value);
    if (symbol->symbolType != VALUE) {
      parseError(astLeaf->value + " is not a readable value");
    } else {
      auto symbolValue = std::static_pointer_cast<SymbolValue>(symbol);
      if (symbolValue->isConst)
        parseError(astLeaf->value + " is a const value");
    }
  });
}
Tokentype ASTWrite::check() {
  ASTListOperation(args, [&](std::shared_ptr<AST> ast) {
    ast->check();
  });
}
Tokentype ASTRet::check() {
  auto type = value->check();
  auto funName = symbolTables.back().tableName;
  auto symbolFunction = std::dynamic_pointer_cast<SymbolFunction>(symbolTables[0].get(funName));
  if (!symbolFunction) {
    throw BaseError("return in a unknown function(maybe the SymbolTable name is wrong?)");
  } else if (symbolFunction->returnType == VOID) {
    parseError("return in a void function");
  }
  if (type != INT && type != CHAR)
    parseError("you can only return int or char");
}
Tokentype ASTSwitch::check() {
  auto type = expression->check();
  if (type != INT && type != CHAR) {
    parseError("object of switch must be int or char");
  }
  ASTListOperation(cases, [&](std::shared_ptr<AST> ast) {
    ast->check();
  });
}

#define toASTImplementHead(type) std::shared_ptr<type> type##toAST(const json &j)

toASTImplementHead(AST) {
  if (j.is_null())return nullptr;
  if (j.is_array()) {
    auto ret = toAST(AST)(j[0]);
    auto tail = ret;
    for (int i = 1; i < j.size(); i++) {
      tail->next = toAST(AST)(j[i]);
      tail = tail->next;
    }
    return ret;
  }
  static std::unordered_map<std::string, std::function<std::shared_ptr<AST>(const json &)>> name2Fun;
  if (name2Fun.empty()) {
    name2Fun["function declare"] = toAST(ASTDeclareFun);
    name2Fun["value declare"] = toAST(ASTDeclareValue);
    name2Fun["array declare"] = toAST(ASTDeclareArray);
    name2Fun["condition"] = toAST(ASTCondition);
    name2Fun["loop"] = toAST(ASTLoop);
    name2Fun["call"] = toAST(ASTCall);
    name2Fun["expression"] = toAST(ASTExpression);
    name2Fun["leaf"] = toAST(ASTLeaf);
    name2Fun["read"] = toAST(ASTRead);
    name2Fun["write"] = toAST(ASTWrite);
    name2Fun["ret"] = toAST(ASTRet);
    name2Fun["switch"] = toAST(ASTSwitch);
  }
  if (j.find("name") == j.end()) {
    parseError("AST ERROR: this is not a standard AST json where is your name!!!");
  }
  auto name = j["name"].get<std::string>();
  return name2Fun[name](j);
}

toASTImplementHead(ASTDeclare) {
  if (j.is_null())return nullptr;
  if (j.is_array()) {
    auto ret = toAST(ASTDeclare)(j[0]);
    std::shared_ptr<AST> tail = ret;
    for (int i = 1; i < j.size(); i++) {
      tail->next = toAST(ASTDeclare)(j[i]);
      tail = tail->next;
    }
    return ret;
  }
  static std::unordered_map<std::string, std::function<std::shared_ptr<ASTDeclare>(const json &)>> name2Fun;
  if (name2Fun.empty()) {
    name2Fun["function declare"] = toAST(ASTDeclareFun);
    name2Fun["value declare"] = toAST(ASTDeclareValue);
    name2Fun["array declare"] = toAST(ASTDeclareArray);
  }
  if (j.find("name") == j.end()) {
    parseError("AST ERROR: this is not a standard AST json where is your name!!!");
  }
  auto name = j["name"].get<std::string>();
  return name2Fun[name](j);
}

toASTImplementHead(ASTDeclareFun) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTDeclareFun>();
  ret->returnType = toTokenType(j["returnType"].get<std::string>());
  ret->body = toAST(AST)(j["body"]);
  ret->valueId = j["valueId"].get<std::string>();
  if (!j["args"].is_null())
    for (auto x:j["args"]) {
      ret->args.emplace_back(toTokenType(x["valueType"].get<std::string>()), x["valueId"].get<std::string>());
    }
  return ret;
}

toASTImplementHead(ASTDeclareValue) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTDeclareValue>();
  ret->valueId = j["valueId"].get<std::string>();
  ret->isConst = j["const"].get<bool>();
  ret->valueType = toTokenType(j["valueType"].get<std::string>());
  ret->value = j["value"].get<std::string>();
  return ret;
}

toASTImplementHead(ASTDeclareArray) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTDeclareArray>();
  ret->valueId = j["valueId"].get<std::string>();
  ret->length = j["length"].get<int>();
  ret->valueType = toTokenType(j["valueType"].get<std::string>());
  return ret;
}

toASTImplementHead(ASTCondition) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTCondition>();
  ret->cmp = toAST(ASTExpression)(j["cmp"]);
  ret->thenStatements = toAST(AST)(j["then"]);
  ret->elseStatements = toAST(AST)(j["else"]);
  return ret;
}

toASTImplementHead(ASTLoop) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTLoop>();
  ret->cmp = toAST(ASTExpression)(j["cmp"]);
  ret->body = toAST(AST)(j["body"]);
  return ret;
}

toASTImplementHead(ASTCall) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTCall>();
  ret->funName = j["funName"].get<std::string>();
  ret->args = toAST(AST)(j["args"]);
  return ret;
}

toASTImplementHead(ASTExpression) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTExpression>();
  ret->operatorType = toTokenType(j["operatorType"].get<std::string>());
  ret->expression1 = toAST(AST)(j["expression1"]);
  ret->expression2 = toAST(AST)(j["expression2"]);
  return ret;
}

toASTImplementHead(ASTLeaf) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTLeaf>();
  ret->value = j["value"].get<std::string>();
  ret->valueType = toTokenType(j["valueType"].get<std::string>());
  return ret;
}

toASTImplementHead(ASTRead) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTRead>();
  ret->args = std::dynamic_pointer_cast<ASTLeaf>(toAST(AST)(j["args"]));
  return ret;
}

toASTImplementHead(ASTWrite) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTWrite>();
  ret->args = toAST(AST)(j["args"]);
  return ret;
}

toASTImplementHead(ASTRet) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTRet>();
  ret->value = toAST(AST)(j["value"]);
  return ret;
}

toASTImplementHead(ASTSwitch) {
  if (j.is_null())return nullptr;
  auto ret = std::make_shared<ASTSwitch>();
  ret->cases = toAST(AST)(j["cases"]);
  ret->expression = toAST(AST)(j["expression"]);
  return ret;
}

address globalName(std::string name, Tokentype tokentype = INT) {
  std::string addr = "g";
  if (tokentype == CHAR)addr += "c";
  else addr += "i";
  addr += name;
  return address(addr);
}

address localName(std::string name, Tokentype tokentype = INT) {
  std::string addr = "l";
  if (tokentype == CHAR)addr += "c";
  else addr += "i";
  addr += name;
  return address(addr);
}

bool isAddress(address addr) {
  return addr[0] == 'l' || addr[0] == 'g' || addr[0] == 't';
}

address ASTDeclareFun::toTAC(TAClist &result, TransInfo transInfo) {
  symbolTables.emplace_back(*(std::dynamic_pointer_cast<SymbolFunction>(symbolTables.front().get(valueId))->funSymbolTable));
  int cnt = 0;
  result.push_back(createTAC<TACLABEL>(valueId));
  for (auto[tokentype, argName]:args) {
    switch (tokentype) {
      case INT:argName = localName(argName);
        result.push_back(createTAC<TACDECLAREINT>(argName));
        break;
      case CHAR:argName = localName(argName, CHAR);
        result.push_back(createTAC<TACDECLARECHAR>(argName));
        break;
    }
    result.push_back(createTAC<TACGETARG>(std::to_string(++cnt), argName));
  }
  transInfo.labelGenerator->addLabel(valueId);
  address returnLabel = transInfo.labelGenerator->newLabel();
  transInfo.returnLabel = returnLabel;
  ASTListOperation(body, [&](std::shared_ptr<AST> ast) {
    ast->toTAC(result, transInfo);
  });
  result.push_back(createTAC<TACLABEL>(returnLabel));
  result.push_back(createTAC<TACRET>());
  symbolTables.pop_back();
  return addrUNDEFINED;
}
address ASTDeclareValue::toTAC(TAClist &result, TransInfo transInfo) {
  if (!isConst) {
    if (symbolTables.size() == 2)
      switch (valueType) {
        case INT:result.push_back(createTAC<TACDECLAREINT>(localName(valueId)));
          break;
        case CHAR:result.push_back(createTAC<TACDECLARECHAR>(localName(valueId, CHAR)));
          break;
      }
    else
      switch (valueType) {
        case INT:result.push_back(createTAC<TACDECLAREINT>(globalName(valueId)));
          break;
        case CHAR:result.push_back(createTAC<TACDECLARECHAR>(globalName(valueId, CHAR)));
          break;
      }
  }
  return addrUNDEFINED;
}
address ASTDeclareArray::toTAC(TAClist &result, TransInfo transInfo) {
  if (symbolTables.size() == 2)
    switch (valueType) {
      case INT:result.push_back(createTAC<TACDECLAREARRAYINT>(address(std::to_string(length)), localName(valueId)));
        break;
      case CHAR:
        result.push_back(createTAC<TACDECLAREARRAYCHAR>(address(std::to_string(length)),
                                                        localName(valueId, CHAR)));
        break;
    }
  else
    switch (valueType) {
      case INT:result.push_back(createTAC<TACDECLAREARRAYINT>(address(std::to_string(length)), globalName(valueId)));
        break;
      case CHAR:
        result.push_back(createTAC<TACDECLAREARRAYCHAR>(address(std::to_string(length)),
                                                        globalName(valueId, CHAR)));
        break;
    }
  return addrUNDEFINED;
}
/*
 * 1. cmp thenLabel then JendLabel elseLabel else endLabel
 * 2. cmp thenLabel then elseLabel
 */
address ASTCondition::toTAC(TAClist &result, TransInfo transInfo) {
  address thenLabel = transInfo.labelGenerator->newLabel();
  address elseLabel = transInfo.labelGenerator->newLabel();
  transInfo.thenLabel = thenLabel;
  transInfo.elseLabel = elseLabel;
  cmp->toTAC(result, transInfo);
  result.push_back(createTAC<TACLABEL>(thenLabel));
  ASTListOperation(thenStatements, [&](std::shared_ptr<AST> ast) { ast->toTAC(result, transInfo); });
  if (elseStatements) {
    address endLabel = transInfo.labelGenerator->newLabel();
    result.push_back(createTAC<TACJ>(endLabel));
    result.push_back(createTAC<TACLABEL>(elseLabel));
    ASTListOperation(elseStatements, [&](std::shared_ptr<AST> ast) { ast->toTAC(result, transInfo); });
    result.push_back(createTAC<TACLABEL>(endLabel));
  } else {
    result.push_back(createTAC<TACLABEL>(elseLabel));
  }
  return addrUNDEFINED;
}
/*
 * beginLabel cmp thenLabel then jbegin endLabel
 */
address ASTLoop::toTAC(TAClist &result, TransInfo transInfo) {
  address beginLabel = transInfo.labelGenerator->newLabel();
  result.push_back(createTAC<TACLABEL>(beginLabel));
  address thenLabel = transInfo.labelGenerator->newLabel();
  address endLabel = transInfo.labelGenerator->newLabel();
  transInfo.thenLabel = thenLabel;
  transInfo.elseLabel = endLabel;
  cmp->toTAC(result, transInfo);
  result.push_back(createTAC<TACLABEL>(thenLabel));
  ASTListOperation(body, [&](std::shared_ptr<AST> ast) { ast->toTAC(result, transInfo); });
  result.push_back(createTAC<TACJ>(beginLabel));
  result.push_back(createTAC<TACLABEL>(endLabel));
  return addrUNDEFINED;
}
/*
 * 1. cal args
 * 2. call
 * 3. return addrUNDEFINED
 */
address ASTCall::toTAC(TAClist &result, TransInfo transInfo) {
  int cnt = 0;
  TAClist tmp;
  ASTListOperation(args, [&](std::shared_ptr<AST> ast) {
    auto addr = ast->toTAC(result, transInfo);
    tmp.push_back(createTAC<TACSETARG>(address(std::to_string(++cnt)), addr));
  });
  result.splice(result.end(), tmp);
  result.push_back(createTAC<TACCALL>(funName));
  address addr = transInfo.addressGenerator->newTmpAddr();
  result.push_back(createTAC<TACGETRET>(addr));
  return addr;
}
address ASTExpression::toTAC(TAClist &result, TransInfo transInfo) {
  if (operatorType == UNDEFINED)return addrUNDEFINED;
  if (!expression1)parseError("unreachable");
  if (operatorType == NOT) {
    std::swap(transInfo.thenLabel, transInfo.elseLabel);
    expression1->toTAC(result, transInfo);
    return addrUNDEFINED;
  } else if (operatorType == AND || operatorType == OR) {
    if (!expression2)parseError("unreachable");
    address exp2Label = transInfo.labelGenerator->newLabel();
    TAClist exp2;
    expression2->toTAC(exp2, transInfo);
    if (operatorType == AND)
      transInfo.thenLabel = exp2Label;
    else
      transInfo.elseLabel = exp2Label;
    expression1->toTAC(result, transInfo);
    result.push_back(createTAC<TACLABEL>(exp2Label));
    result.splice(result.end(), exp2);
    return addrUNDEFINED;
  } else if (operatorType == LT || operatorType == GT || operatorType == NE || operatorType == GE || operatorType == LE
      || operatorType == EQ) {
    if (!expression2)parseError("unreachable");
    address exp1 = expression1->toTAC(result, transInfo);
    address exp2 = expression2->toTAC(result, transInfo);
    if (exp1 == addrUNDEFINED || exp2 == addrUNDEFINED)parseError("unreachable");
    if (!isAddress(exp1) && !isAddress(exp2)) {
      bool flag;
      switch (operatorType) {
        case LT:flag = (atoi(exp1.c_str()) < atoi(exp2.c_str()));
          break;
        case LE:flag = (atoi(exp1.c_str()) <= atoi(exp2.c_str()));
          break;
        case GT:flag = (atoi(exp1.c_str()) > atoi(exp2.c_str()));
          break;
        case GE:flag = (atoi(exp1.c_str()) >= atoi(exp2.c_str()));
          break;
        case NE:flag = (atoi(exp1.c_str()) != atoi(exp2.c_str()));
          break;
        case EQ:flag = (atoi(exp1.c_str()) == atoi(exp2.c_str()));
          break;
      }
      if (flag)result.push_back(createTAC<TACJ>(transInfo.thenLabel));
      else result.push_back(createTAC<TACJ>(transInfo.elseLabel));
    } else {
      switch (operatorType) {
        case LT:result.push_back(createTAC<TACJL>(exp1, exp2, transInfo.thenLabel));
          break;
        case LE:result.push_back(createTAC<TACJLE>(exp1, exp2, transInfo.thenLabel));
          break;
        case GT:result.push_back(createTAC<TACJG>(exp1, exp2, transInfo.thenLabel));
          break;
        case GE:result.push_back(createTAC<TACJGE>(exp1, exp2, transInfo.thenLabel));
          break;
        case NE:result.push_back(createTAC<TACJNE>(exp1, exp2, transInfo.thenLabel));
          break;
        case EQ:result.push_back(createTAC<TACJEQ>(exp1, exp2, transInfo.thenLabel));
          break;
      }
      result.push_back(createTAC<TACJ>(transInfo.elseLabel));
    }
    return addrUNDEFINED;
  } else if (operatorType == MINUS) {
    address exp1 = expression1->toTAC(result, transInfo);
    if (exp1 == addrUNDEFINED)parseError("unreachable");
    if (expression2) {
      address exp2 = expression2->toTAC(result, transInfo);
      if (exp2 == addrUNDEFINED)parseError("unreachable");
      if (!isAddress(exp1) && !isAddress(exp2)) {
        return address(std::to_string(atoi(exp1.c_str()) - atoi(exp2.c_str())));
      } else {
        address addr = transInfo.addressGenerator->newTmpAddr();
        result.push_back(createTAC<TACSUB>(exp1, exp2, addr));
        return addr;
      }
    } else {
      if (!isAddress(exp1)) {
        return address(std::to_string(-atoi(exp1.c_str())));
      } else {
        address addr = transInfo.addressGenerator->newTmpAddr();
        result.push_back(createTAC<TACSUB>(address("0"), exp1, addr));
        return addr;
      }
    }
  } else if (operatorType == ADD || operatorType == MUL || operatorType == DIV) {
    if (!expression2)parseError("unreachable");
    address exp1 = expression1->toTAC(result, transInfo);
    address exp2 = expression2->toTAC(result, transInfo);
    if (exp1 == addrUNDEFINED)parseError("unreachable");
    if (exp2 == addrUNDEFINED)parseError("unreachable");
    if (!isAddress(exp1) && !isAddress(exp2)) {
      switch (operatorType) {
        case ADD:return address(std::to_string(atoi(exp1.c_str()) + atoi(exp2.c_str())));
        case MUL:return address(std::to_string(atoi(exp1.c_str()) * atoi(exp2.c_str())));
        case DIV:
          if (atoi(exp2.c_str()))
            return address(std::to_string(atoi(exp1.c_str()) / atoi(exp2.c_str())));
          else
            error("divide by zero error");
      }
    } else {
      address addr = transInfo.addressGenerator->newTmpAddr();
      switch (operatorType) {
        case ADD:result.push_back(createTAC<TACADD>(exp1, exp2, addr));
          return addr;
        case MUL:result.push_back(createTAC<TACMUL>(exp1, exp2, addr));
          return addr;
        case DIV:result.push_back(createTAC<TACDIV>(exp1, exp2, addr));
          return addr;
      }
    }
  } else if (operatorType == ASSIGN) {
    if (!expression2)parseError("unreachable");
    address addr2 = expression2->toTAC(result, transInfo);
    address addr1 = expression1->toTAC(result, transInfo);
    if (addr1 == addrUNDEFINED || addr2 == addrUNDEFINED)parseError("unreachable");
    if (result.back().op == TACGETARR) {
      result.back().op = TACSETARR;
      result.back().ad3 = addr2;
    } else {
      result.push_back(createTAC<TACMOV>(addr2, addr1));
    }
    return addrUNDEFINED;
  } else if (operatorType == ARRAY) {
    if (!expression2)parseError("unreachable");
    address addr2 = expression2->toTAC(result, transInfo);
    address addr1 = expression1->toTAC(result, transInfo);
    if (addr1 == addrUNDEFINED || addr2 == addrUNDEFINED)parseError("unreachable");
    address addr3 = transInfo.addressGenerator->newTmpAddr();
    result.push_back(createTAC<TACGETARR>(addr1, addr2, addr3));
    return addr3;
  } else
    parseError("unreachable");
}
address ASTLeaf::toTAC(TAClist &result, TransInfo transInfo) {
  if (valueType == CHAR || valueType == CHARACTER)return address(std::to_string(realValue(value)));
  if (valueType == STRING)return address(value);
  if (valueType == INT || valueType == UNSIGNED)return address(value);
  if (valueType == ID) {
    auto symbol = symbolTables.back().get(value);
    std::function<address(std::string, Tokentype)> fun;
    if (symbol)fun = localName;
    else {
      symbol = symbolTables.front().get(value);
      if (symbol)fun = globalName;
      else {
        parseError("unreachable");
      }
    }
    if (symbol->symbolType == VALUE) {
      auto symbolValue = std::static_pointer_cast<SymbolValue>(symbol);
      if (symbolValue->isConst)return address(std::to_string(symbolValue->value));
      return fun(value, symbolValue->valueType);
    } else if (symbol->symbolType == ARRAY) {
      auto symbolArray = std::static_pointer_cast<SymbolArray>(symbol);
      return fun(value, symbolArray->valueType);
    } else {
      parseError("unreachable");
    }
  }
  return addrUNDEFINED;
}
address ASTRead::toTAC(TAClist &result, TransInfo transInfo) {
  ASTListOperation(args, [&](std::shared_ptr<AST> ast) {
    address addr = ast->toTAC(result, transInfo);
    if (addr[1] == 'i')result.push_back(createTAC<TACREADINT>(addr));
    else if (addr[1] == 'c')result.push_back(createTAC<TACREADCHAR>(addr));
    else parseError("unreachable");
  });
  return addrUNDEFINED;
}
address ASTWrite::toTAC(TAClist &result, TransInfo transInfo) {
  ASTListOperation(args, [&](std::shared_ptr<AST> ast) {
    address addr = ast->toTAC(result, transInfo);
    if (addr[0] == '"') {
      result.push_back(createTAC<TACDECLARESTRING>(addr));
      result.push_back(createTAC<TACWRITESTRING>(addr));
    } else if (isdigit(addr[0]) || addr[0] == '-' || addr[1] == 'i')result.push_back(createTAC<TACWRITEINT>(addr));
    else if (addr[1] == 'c')result.push_back(createTAC<TACWRITECHAR>(addr));
    else parseError("unreachable");
  });
  return addrUNDEFINED;
}
address ASTRet::toTAC(TAClist &result, TransInfo transInfo) {
  address addr = value->toTAC(result, transInfo);
  result.push_back(createTAC<TACSETRET>(addr));
  result.push_back(createTAC<TACJ>(transInfo.returnLabel));
  return addrUNDEFINED;
}
/*
 * jump table
 * calculate code
 * end
 */
address ASTSwitch::toTAC(TAClist &result, TransInfo transInfo) {
  const int CHANGE = 8;
  std::vector<std::variant<address, TAClist>> cas;
  auto cnt = ASTListOperation(cases);
  address val = expression->toTAC(result, transInfo);
  cnt = ASTListOperation(cases, [&](std::shared_ptr<AST> ast) {
    TAClist tacList;
    address addr = ast->toTAC(tacList, transInfo);
    if (cnt & 1) {
      cas.emplace_back(tacList);
    } else {
      cas.emplace_back(addr);
    }
    cnt--;
  });
  std::vector<std::pair<int, address>> i2label;// constant label
  if (cnt == 1) {
    // only default
    result.splice(result.end(), std::get<TAClist>(cas[0]));
  } else {
    address switchEnd = transInfo.labelGenerator->newLabel();
    int begin = 0;
    TAClist calCode;
    TAClist jumpCode;
    address defaultLabel = addrUNDEFINED;
    if (cnt & 1) {
      begin++;
      defaultLabel = transInfo.labelGenerator->newLabel();
      calCode.push_back(createTAC<TACLABEL>(defaultLabel));
      calCode.splice(calCode.end(), std::get<TAClist>(cas[0]));
      calCode.push_back(createTAC<TACJ>(switchEnd));
    } else {
      defaultLabel = switchEnd;
    }
    for (int i = begin; i < cas.size(); i += 2) {
      address constant = std::get<address>(cas[i]);
      TAClist tacList = std::get<TAClist>(cas[i + 1]);
      address label = transInfo.labelGenerator->newLabel();
      i2label.emplace_back(atoi(constant.c_str()), label);
      calCode.push_back(createTAC<TACLABEL>(label));
      calCode.splice(calCode.end(), tacList);
      calCode.push_back(createTAC<TACJ>(switchEnd));
    }
    std::sort(i2label.begin(), i2label.end());
    if (i2label.size() <= CHANGE) {
      // linear
      for (auto[constatnt, label]:i2label) {
        jumpCode.push_back(createTAC<TACJEQ>(address(std::to_string(constatnt)), val, label));
      }
      jumpCode.push_back(createTAC<TACJ>(defaultLabel));
    } else {
      // binary search
      std::function<TAClist(int, int)> work = [&](int l, int r) -> TAClist {
        TAClist ret;
        if (l > r) {
          ret.push_back(createTAC<TACJ>(defaultLabel));
        } else if (l == r) {
          ret.push_back(createTAC<TACJEQ>(address(std::to_string(i2label[l].first)), val, i2label[l].second));
          ret.push_back(createTAC<TACJ>(defaultLabel));
        } else {
          int mid = (l + r) / 2;
          /*
           * jeq x val eqLabel
           * jg x val gLabel
           * jump l mid-1
           * gLabel
           * jump mid+1 r
           */
          ret.push_back(createTAC<TACJEQ>(address(std::to_string(i2label[mid].first)), val, i2label[mid].second));
          address gLabel = transInfo.labelGenerator->newLabel();
          ret.push_back(createTAC<TACJG>(address(std::to_string(i2label[mid].first)), val, gLabel));
          ret.splice(ret.end(), work(l, mid - 1));
          ret.push_back(createTAC<TACLABEL>(gLabel));
          ret.splice(ret.end(), work(mid + 1, r));
        }
        return ret;
      };
      jumpCode = work(0, i2label.size() - 1);
    }
    result.splice(result.end(), jumpCode);
    result.splice(result.end(), calCode);
    result.push_back(createTAC<TACLABEL>(switchEnd));
  }
  return addrUNDEFINED;
}
