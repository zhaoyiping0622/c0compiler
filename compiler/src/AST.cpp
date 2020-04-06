//
// Created by zhaoyiping on 2020/3/31.
//

#include "AST.h"

AST::AST() : next(nullptr) {}
ASTDeclareFun::ASTDeclareFun() {}
ASTDeclareFun::ASTDeclareFun(Tokentype returntype,
                             std::vector<std::pair<Tokentype, std::string>> args,
                             std::shared_ptr<AST> body) : returnType(returnType), args(args), body(body) {}
ASTDeclareValue::ASTDeclareValue() {}
ASTDeclareValue::ASTDeclareValue(bool isConst, Tokentype valueType, std::string valueId, std::string value)
    : isConst(isConst), valueType(valueType), value(value) { this->valueId = valueId; }
ASTCondition::ASTCondition() {}
ASTCondition::ASTCondition(std::shared_ptr<ASTStatement> cmp,
                           std::shared_ptr<AST> thenStatements,
                           std::shared_ptr<AST> elseStatements)
    : cmp(cmp), thenStatements(thenStatements), elseStatements(elseStatements) {}
ASTLoop::ASTLoop() {}
ASTLoop::ASTLoop(std::shared_ptr<ASTStatement> cmp, std::shared_ptr<AST> statements)
    : cmp(cmp), statements(statements) {}
ASTCall::ASTCall() {}
ASTCall::ASTCall(std::string funName, std::shared_ptr<AST> args)
    : funName(funName), args(args) {}
ASTStatement::ASTStatement() {}
ASTStatement::ASTStatement(Tokentype tokentype,
                           std::shared_ptr<AST> statement1,
                           std::shared_ptr<AST> statement2)
    : tokentype(tokentype), statement1(statement1), statement2(statement2) {}
ASTLeaf::ASTLeaf() {}
ASTLeaf::ASTLeaf(std::string value, Tokentype tokentype) : value(value), valuetype(tokentype) {}
ASTVector::ASTVector(std::vector<std::shared_ptr<AST>> &&v) : v(std::move(v)) {}
ASTVector::ASTVector() {}
ASTRead::ASTRead() {}
ASTRead::ASTRead(std::shared_ptr<ASTLeaf> args) : args(args) {}
ASTWrite::ASTWrite() {}
ASTWrite::ASTWrite(std::shared_ptr<AST> &&args) : args(args) {}
ASTRet::ASTRet() {}
ASTRet::ASTRet(std::shared_ptr<AST> value) : value(value) {}
ASTSwitch::ASTSwitch() {}
ASTSwitch::ASTSwitch(std::shared_ptr<ASTStatement> statement, std::shared_ptr<AST> cases)
    : statement(statement), cases(cases) {}
ASTDeclareArray::ASTDeclareArray() {}
ASTDeclareArray::ASTDeclareArray(int length,
                                 Tokentype valuetype,
                                 std::string valueId)
    : length(length), valueType(valuetype) { this->valueId = valueId; }
