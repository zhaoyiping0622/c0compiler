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
ASTCondition::ASTCondition(std::shared_ptr<ASTStatement> cmp,
                           std::shared_ptr<AST> thenStatements,
                           std::shared_ptr<AST> elseStatements)
    : cmp(cmp), thenStatements(thenStatements), elseStatements(elseStatements) {}
ASTLoop::ASTLoop() {}
ASTLoop::ASTLoop(std::shared_ptr<ASTStatement> cmp, std::shared_ptr<AST> statements)
    : cmp(cmp), body(statements) {}
ASTCall::ASTCall() {}
ASTCall::ASTCall(std::string funName, std::shared_ptr<AST> args)
    : funName(funName), args(args) {}
ASTStatement::ASTStatement() {}
ASTStatement::ASTStatement(Tokentype operatorType,
                           std::shared_ptr<AST> statement1,
                           std::shared_ptr<AST> statement2)
    : operatorType(operatorType), statement1(statement1), statement2(statement2) {}
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

to_jsonImplement(AST)
to_jsonImplement(ASTDeclare)
to_jsonImplement(ASTDeclareFun)
to_jsonImplement(ASTDeclareValue)
to_jsonImplement(ASTDeclareArray)
to_jsonImplement(ASTCondition)
to_jsonImplement(ASTLoop)
to_jsonImplement(ASTCall)
to_jsonImplement(ASTStatement)
to_jsonImplement(ASTLeaf)
to_jsonImplement(ASTRead)
to_jsonImplement(ASTWrite)
to_jsonImplement(ASTRet)
to_jsonImplement(ASTSwitch)

json ASTDeclareFun::toJSON() {
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
  ret["body"] = body;
  ret["valueId"] = valueId;
  ret["next"] = next;
  return ret;
}

json ASTDeclareValue::toJSON() {
  json ret;
  ret["name"] = "value declare";
  ret["valueId"] = valueId;
  ret["const"] = isConst;
  ret["valueType"] = toString(valueType);
  ret["value"] = value;
  ret["next"] = next;
  return ret;
}

json ASTDeclareArray::toJSON() {
  json ret;
  ret["name"] = "array declare";
  ret["valueId"] = valueId;
  ret["length"] = length;
  ret["valueType"] = toString(valueType);
  ret["next"] = next;
  return ret;
}

json ASTCondition::toJSON() {
  json ret;
  ret["name"] = "condition";
  ret["cmp"] = cmp;
  ret["then"] = thenStatements;
  ret["else"] = elseStatements;
  ret["next"] = next;
  return ret;
}

json ASTLoop::toJSON() {
  json ret;
  ret["name"] = "loop";
  ret["cmp"] = cmp;
  ret["body"] = body;
  ret["next"] = next;
  return ret;
}

json ASTCall::toJSON() {
  json ret;
  ret["name"] = "call";
  ret["funName"] = funName;
  ret["args"] = args;
  ret["next"] = next;
  return ret;
}

json ASTStatement::toJSON() {
  json ret;
  ret["name"] = "statement";
  ret["operatorType"] = toString(operatorType);
  ret["statement1"] = statement1;
  ret["statement2"] = statement2;
  ret["next"] = next;
  return ret;
}

json ASTLeaf::toJSON() {
  json ret;
  ret["name"] = "leaf";
  ret["value"] = value;
  ret["valueType"] = toString(valueType);
  ret["next"] = next;
  return ret;
}

json ASTRead::toJSON() {
  json ret;
  ret["name"] = "read";
  ret["args"] = args;
  ret["next"] = next;
  return ret;
}

json ASTWrite::toJSON() {
  json ret;
  ret["name"] = "write";
  ret["args"] = args;
  ret["next"] = next;
  return ret;
}

json ASTRet::toJSON() {
  json ret;
  ret["name"] = "ret";
  ret["value"] = value;
  ret["next"] = next;
  return ret;
}

json ASTSwitch::toJSON() {
  json ret;
  ret["name"] = "switch";
  ret["cases"] = cases;
  ret["expression"] = expression;
  ret["next"] = next;
  return ret;
}