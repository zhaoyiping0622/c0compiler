//
// Created by zhaoyiping on 2020/4/7.
//
#include "gtest/gtest.h"
#include "AST.h"

TEST(ASTTestJSON, DeclareFunNoArg) {
  auto ast = std::make_shared<ASTDeclareFun>
      (VOID,
       std::vector<std::pair<Tokentype, std::string>>(),
       std::shared_ptr<AST>(nullptr),
       "test");
  json j;
  j["name"] = "function declare";
  j["returnType"] = toString(VOID);
  j["args"] = nullptr;
  j["body"] = nullptr;
  j["valueId"] = "test";
  json j2 = ast;
  ASSERT_EQ(j, j2);
}
TEST(ASTTestJSON, DeclareFun1Arg) {
  auto ast = std::make_shared<ASTDeclareFun>
      (INT,
       std::vector<std::pair<Tokentype, std::string>>
           ({std::make_pair(INT, "abc")}),
       std::shared_ptr<AST>(nullptr),
       "test");
  ast->valueId = "test";
  json j;
  j["name"] = "function declare";
  j["returnType"] = toString(INT);
  j["args"][0] = {{"valueType", toString(INT)}, {"valueId", "abc"}};
  j["body"] = nullptr;
  j["valueId"] = "test";
  json j2 = ast;
  ASSERT_EQ(j, j2);
}
TEST(ASTTestJSON, DeclareFunManyArg) {
  auto ast = std::make_shared<ASTDeclareFun>
      (INT,
       std::vector<std::pair<Tokentype, std::string>>
           ({std::make_pair(INT, "abc"),
             std::make_pair(CHAR, "def")}),
       std::shared_ptr<AST>(nullptr),
       "test");
  json j;
  j["returnType"] = toString(INT);
  j["name"] = "function declare";
  j["args"][0] = {{"valueType", toString(INT)}, {"valueId", "abc"}};
  j["args"][1] = {{"valueType", toString(CHAR)}, {"valueId", "def"}};
  j["body"] = nullptr;
  j["valueId"] = "test";
  json j2 = ast;
  ASSERT_EQ(j, j2);
  j["args"][1] = {{"valueType", toString(INT)}, {"valueId", "abc"}};
  j["args"][0] = {{"valueType", toString(CHAR)}, {"valueId", "def"}};
  ASSERT_NE(j, j2);
}
TEST(ASTTestJSON, DeclareValueINT) {
  auto ast = std::make_shared<ASTDeclareValue>
      (true, INT, "abc", "123");
  json j;
  j["name"] = "value declare";
  j["valueId"] = "abc";
  j["const"] = true;
  j["valueType"] = toString(INT);
  j["value"] = "123";
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, DeclareValueCHAR) {
  auto ast = std::make_shared<ASTDeclareValue>
      (true, CHAR, "abc", "\\\'");
  json j;
  j["name"] = "value declare";
  j["valueId"] = "abc";
  j["const"] = true;
  j["valueType"] = toString(CHAR);
  j["value"] = "\\\'";
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, DeclareArray) {
  auto ast = std::make_shared<ASTDeclareArray>
      (10, INT, "abc");
  json j;
  j["name"] = "array declare";
  j["valueId"] = "abc";
  j["length"] = 10;
  j["valueType"] = toString(INT);
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, Condition) {
  auto ast = std::make_shared<ASTCondition>(nullptr, nullptr, nullptr);
  json j;
  j["name"] = "condition";
  j["cmp"] = nullptr;
  j["then"] = nullptr;
  j["else"] = nullptr;
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, Loop) {
  auto ast = std::make_shared<ASTLoop>(nullptr, nullptr);
  json j;
  j["name"] = "loop";
  j["cmp"] = nullptr;
  j["body"] = nullptr;
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, Call) {
  auto ast = std::make_shared<ASTCall>("test", nullptr);
  json j;
  j["name"] = "call";
  j["funName"] = "test";
  j["args"] = nullptr;
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, Statement) {
  auto ast = std::make_shared<ASTExpression>(ADD, nullptr, nullptr);
  json j;
  j["name"] = "expression";
  j["operatorType"] = toString(ADD);
  j["expression1"] = nullptr;
  j["expression2"] = nullptr;
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, Leaf) {
  auto ast = std::make_shared<ASTLeaf>("abc", INT);
  json j;
  j["name"] = "leaf";
  j["value"] = "abc";
  j["valueType"] = toString(INT);
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, Read) {
  auto ast = std::make_shared<ASTRead>(nullptr);
  json j;
  j["name"] = "read";
  j["args"] = nullptr;
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, Write) {
  auto ast = std::make_shared<ASTWrite>(nullptr);
  json j;
  j["name"] = "write";
  j["args"] = nullptr;
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, Ret) {
  auto ast = std::make_shared<ASTRet>(nullptr);
  json j;
  j["name"] = "ret";
  j["value"] = nullptr;
  ASSERT_EQ(j, ast->toJSON());
}
TEST(ASTTestJSON, Switch) {
  auto ast = std::make_shared<ASTSwitch>(nullptr, nullptr);
  json j;
  j["name"] = "switch";
  j["cases"] = nullptr;
  j["expression"] = nullptr;
  ASSERT_EQ(j, ast->toJSON());
}
