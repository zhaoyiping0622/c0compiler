//
// Created by zhaoyiping on 2020/3/31.
//

#include "AST.h"

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
                             std::shared_ptr<AST> statement1,
                             std::shared_ptr<AST> statement2)
    : operatorType(operatorType), expression1(statement1), expression2(statement2) {}
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
  ret["body"] = ::toJSON(body,true);
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
  ret["then"] = ::toJSON(thenStatements,true);
  ret["else"] = ::toJSON(elseStatements,true);
  makeArray;
  return ret;
}

json ASTLoop::toJSON(bool root) {
  json ret;
  ret["name"] = "loop";
  ret["cmp"] = cmp;
  ret["body"] = ::toJSON(body,true);
  makeArray;
  return ret;
}

json ASTCall::toJSON(bool root) {
  json ret;
  ret["name"] = "call";
  ret["funName"] = funName;
  ret["args"] = ::toJSON(args,true);
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
  ret["args"] = ::toJSON(args,true);
  makeArray;
  return ret;
}

json ASTWrite::toJSON(bool root) {
  json ret;
  ret["name"] = "write";
  ret["args"] = ::toJSON(args,true);
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
  ret["cases"] = ::toJSON(cases,true);
  ret["expression"] = expression;
  makeArray;
  return ret;
}