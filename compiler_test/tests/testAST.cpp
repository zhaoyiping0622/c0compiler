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

void testASTEqual(std::shared_ptr<AST> a) {
  json j = a->toJSON();
  json j1 = toAST(AST)(a)->toJSON();
  ASSERT_EQ(j.dump(2), j1.dump(2));
}

TEST(ASTTestJSON, ASTDeclareFun) {
  auto root = std::make_shared<ASTDeclareFun>(VOID, std::vector<std::pair<Tokentype, std::string>>(), nullptr, "fun");
  testASTEqual(root);
  auto body = std::make_shared<ASTDeclareValue>(false, INT, "ABC", "123");
  std::shared_ptr<AST> tail = body;
  tail = tail->next = std::make_shared<ASTDeclareArray>(10, INT, "ABC");
  auto args1 = std::vector<std::pair<Tokentype, std::string>>(
      {
          {INT, "a"},
      });
  auto args2 = std::vector<std::pair<Tokentype, std::string>>(
      {
          {INT, "a"},
          {CHAR, "b"}
      });
  auto args3 = std::vector<std::pair<Tokentype, std::string>>(
      {
          {INT, "a"},
          {CHAR, "b"},
          {INT, "c"},
      });
  root = std::make_shared<ASTDeclareFun>(VOID, args1, tail, "fun");
  testASTEqual(root);
  root = std::make_shared<ASTDeclareFun>(INT, args2, body, "abc");
  testASTEqual(root);
  root = std::make_shared<ASTDeclareFun>(CHAR, args3, body, "cde");
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTDeclareValue) {
  auto root = std::make_shared<ASTDeclareValue>(false, INT, "abc", "123");
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTDeclareArray) {
  auto root = std::make_shared<ASTDeclareArray>(10, INT, "ABC");
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTDeclare) {
  auto fun = std::make_shared<ASTDeclareFun>(VOID, std::vector<std::pair<Tokentype, std::string>>(), nullptr, "fun");
  auto value = std::make_shared<ASTDeclareValue>(false, INT, "abc", "123");
  auto array = std::make_shared<ASTDeclareArray>(10, INT, "ABC");
  testASTEqual(fun);
  testASTEqual(value);
  testASTEqual(array);
}
TEST(ASTTestJSON, ASTCondition) {
  auto root = std::make_shared<ASTCondition>();
  testASTEqual(root);
  auto expression = std::make_shared<ASTExpression>(
      ADD,
      std::make_shared<ASTLeaf>("1", INT),
      std::make_shared<ASTLeaf>("'1'", CHAR)
  );
  root = std::make_shared<ASTCondition>(expression, expression, expression);
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTLoop) {
  auto root = std::make_shared<ASTLoop>();
  auto expression = std::make_shared<ASTExpression>(
      ADD,
      std::make_shared<ASTLeaf>("1", INT),
      std::make_shared<ASTLeaf>("'1'", CHAR)
  );
  testASTEqual(root);
  root = std::make_shared<ASTLoop>(expression, expression);
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTCall) {
  auto arg1 = std::make_shared<ASTLeaf>("a", INT);
  auto arg2 = std::make_shared<ASTLeaf>("b", CHAR);
  auto arg3 = std::make_shared<ASTLeaf>("c", INT);
  arg1->next = arg2;
  arg2->next = arg3;
  auto root = std::make_shared<ASTCall>("abc", arg1);
  testASTEqual(root);
  root->args = arg2;
  testASTEqual(root);
  root->args = arg3;
  testASTEqual(root);
  root->args = nullptr;
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTExpression) {
  auto root = std::make_shared<ASTExpression>
      (OR,
       std::make_shared<ASTLeaf>("1", INT),
       std::make_shared<ASTLeaf>("'1'", CHAR));
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTLeaf) {
  auto root = std::make_shared<ASTLeaf>("123", INT);
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTRead) {
  auto arg1 = std::make_shared<ASTLeaf>("a", ID);
  auto arg2 = std::make_shared<ASTLeaf>("b", ID);
  auto arg3 = std::make_shared<ASTLeaf>("c", ID);
  arg1->next = arg2;
  arg2->next = arg3;
  auto root = std::make_shared<ASTRead>(arg1);
  testASTEqual(root);
  root->args = arg2;
  testASTEqual(root);
  root->args = arg3;
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTWrite) {
  auto arg1 = std::make_shared<ASTLeaf>("abcdef", STRING);
  auto arg2 = std::make_shared<ASTLeaf>("123", INT);
  auto arg3 = std::make_shared<ASTExpression>(ADD, arg1, arg2);
  arg1->next = arg2;
  arg2->next = arg3;
  auto root = std::make_shared<ASTWrite>(arg1);
  testASTEqual(root);
  root->args = arg2;
  testASTEqual(root);
  root->args = arg3;
  testASTEqual(root);
}
TEST(ASTTestJSON, ASTRet) {
  auto arg1 = std::make_shared<ASTLeaf>("1", INT);
  auto arg2 = std::make_shared<ASTLeaf>("'1'", CHAR);
  auto arg3 = std::make_shared<ASTExpression>(ADD, arg1, arg2);
  testASTEqual(std::make_shared<ASTRet>(arg1));
  testASTEqual(std::make_shared<ASTRet>(arg2));
  testASTEqual(std::make_shared<ASTRet>(arg3));
}
TEST(ASTTestJSON, ASTSwitch) {
  auto arg1 = std::make_shared<ASTLeaf>("123", UNSIGNED);
  auto arg2 = std::make_shared<ASTLeaf>("'1'", CHARACTER);
  auto arg3 = std::make_shared<ASTExpression>(MINUS, arg1, nullptr);
  auto aplusb = std::make_shared<ASTExpression>(ADD, arg1, arg2);
  auto Default = aplusb;
  testASTEqual(std::make_shared<ASTSwitch>(aplusb, Default));
  Default->next = arg3;
  arg3->next = arg1;
  testASTEqual(std::make_shared<ASTSwitch>(aplusb, Default));
}

