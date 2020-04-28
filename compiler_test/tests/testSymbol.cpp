//
// Created by zhaoyiping on 2020/4/19.
//

#include "token.h"
#include "symbol.h"
#include "test.h"

TEST(SymbolTableTest, getAndInsert) {
  SymbolTable testSymbolTable;
  std::shared_ptr<SymbolValue> valuePtr;
  ASSERT_NE(valuePtr = testSymbolTable.insert<SymbolValue>("abc"), nullptr);
  ASSERT_EQ(valuePtr, testSymbolTable.get("abc"));
  ASSERT_EQ(testSymbolTable.insert<SymbolValue>("abc"), nullptr);
  testSymbolTable.clear();
  std::shared_ptr<SymbolArray> arrayPtr;
  ASSERT_NE(arrayPtr = testSymbolTable.insert<SymbolArray>("abc"), nullptr);
  ASSERT_EQ(arrayPtr, testSymbolTable.get("abc"));
  ASSERT_EQ(testSymbolTable.insert<SymbolArray>("abc"), nullptr);
  std::shared_ptr<SymbolFunction> functionPtr;
  ASSERT_NE(functionPtr = testSymbolTable.insert<SymbolFunction>("def"), nullptr);
  ASSERT_EQ(functionPtr, testSymbolTable.get("def"));
  ASSERT_EQ(testSymbolTable.insert<SymbolFunction>("def"), nullptr);
}

TEST(SymbolTest, value) {
  auto x = SymbolValue(INT);
  ASSERT_EQ(x.symbolType, VALUE);
  ASSERT_EQ(x.value, 0);
  ASSERT_EQ(x.isConst, false);
  ASSERT_EQ(x.valueType, INT);
  x = SymbolValue(CHAR, true, 1);
  ASSERT_EQ(x.symbolType, VALUE);
  ASSERT_EQ(x.value, 1);
  ASSERT_EQ(x.isConst, true);
  ASSERT_EQ(x.valueType, CHAR);
}

TEST(SymbolTest, array) {
  auto x = SymbolArray(INT, 10);
  ASSERT_EQ(x.symbolType, ARRAY);
  ASSERT_EQ(x.valueType, INT);
  ASSERT_EQ(x.size, 10);
}

TEST(SymbolTest, function) {
  auto x = SymbolFunction();
  ASSERT_EQ(x.argType.size(), 0);
  x = SymbolFunction({INT, CHAR, INT}, VOID);
  ASSERT_EQ(x.returnType, VOID);
  std::vector<Tokentype> v = {INT, CHAR, INT};
  ASSERT_EQ(x.argType, v);
}