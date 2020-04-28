//
// Created by zhaoyiping on 2020/3/31.
//

#include "symbol.h"

#include "token.h"
Symbol::Symbol() {}
Symbol::Symbol(Tokentype symbolType) : symbolType(symbolType) {}
SymbolValue::SymbolValue() : Symbol(VALUE) {}
SymbolValue::SymbolValue(Tokentype valueType, bool isConst, int value)
    : Symbol(VALUE), valueType(valueType), isConst(isConst), value(value) {}
SymbolArray::SymbolArray() : Symbol(ARRAY) {}
SymbolArray::SymbolArray(Tokentype valueType, int size)
    : Symbol(ARRAY), valueType(valueType), size(size) {}
SymbolFunction::SymbolFunction() : Symbol(FUNCTION) {}
SymbolFunction::SymbolFunction(std::vector<Tokentype> arg_type, Tokentype returnType)
    : Symbol(FUNCTION), argType(std::move(arg_type)), returnType(returnType) {
  funSymbolTable = std::make_shared<SymbolTable>();
}
std::shared_ptr<Symbol> SymbolTable::get(const std::string &s) {
  auto it = table.find(s);
  if (it == table.end())
    return nullptr;
  else
    return it->second;
}
void SymbolTable::clear() {
  table.clear();
}
bool Symbol::operator==(const Symbol &other) const {
  if (symbolType != other.symbolType)
    return false;
  if (symbolType == VALUE)
    return *static_cast<const SymbolValue *>(this) == static_cast<const SymbolValue &>(other);
  else if (symbolType == ARRAY)
    return *static_cast<const SymbolArray *>(this) == static_cast<const SymbolArray &>(other);
  else if (symbolType == FUNCTION)
    return *static_cast<const SymbolFunction *>(this) == static_cast<const SymbolFunction &>(other);
}

bool SymbolValue::operator==(const SymbolValue &other) const {
  return valueType == other.valueType && isConst == other.isConst && value == other.value;
}

bool SymbolArray::operator==(const SymbolArray &other) const {
  return valueType == other.valueType && size == other.size;
}

bool SymbolFunction::operator==(const SymbolFunction &other) const {
  return returnType == other.returnType
      && argType == other.argType
      && *(funSymbolTable.get()) == *(other.funSymbolTable.get());
}
bool operator==(const SymbolTable &a, const SymbolTable &b) {
  if (a.tableName != b.tableName)return false;
  if (a.table.size() != b.table.size())return false;
  for (const auto&[key, value]:a.table) {
    if (b.table.find(key) == b.table.end())return false;
    auto value1 = b.table.find(key)->second;
    if (!(*value == *value1))
      return false;
  }
  return true;
}
SymbolTable::SymbolTable(std::string table_name) : tableName(table_name) {}
