//
// Created by zhaoyiping on 2020/3/31.
//

#ifndef COMPILER_COMPILER_INCLUDE_SYMBOL_H_
#define COMPILER_COMPILER_INCLUDE_SYMBOL_H_

#include "base.h"
#include "vector"
#include "unordered_map"
#include "memory"

class Symbol {
 public:
  Tokentype symbolType;
  Symbol(Tokentype symbolType);
  Symbol();
  bool operator==(const Symbol &other) const;
  virtual ~Symbol() {}
};

class SymbolTable {
 public:
  std::string tableName;
  /*
   * if string in symbol table return the pointer to symbol
   * else return nullptr
   */
  std::shared_ptr<Symbol> get(const std::string &);
  SymbolTable(std::string table_name = "global");
  /*
   * insert once
   * reinsert will return nullptr
   */
  template<typename SymbolType, typename ... Args>
  std::shared_ptr<SymbolType> insert(const std::string &s, const Args &... args) {
    if (get(s))
      return nullptr;
    auto ret = std::make_shared<SymbolType>(args...);
    table[s] = ret;
    return ret;
  }
  void clear();
 private:
  std::unordered_map<std::string, std::shared_ptr<Symbol>> table;
  friend bool operator==(const SymbolTable &a, const SymbolTable &b);
};

class SymbolValue : public Symbol {
 public:
  Tokentype valueType;
  bool isConst;
  int value;
  SymbolValue(Tokentype valueType, bool isConst = false, int value = 0);
  SymbolValue();
  bool operator==(const SymbolValue &other) const;
};

class SymbolArray : public Symbol {
 public:
  SymbolArray();
  SymbolArray(Tokentype valueType, int size);
 public:
  Tokentype valueType;
  int size;
  bool operator==(const SymbolArray &other) const;
};

class SymbolFunction : public Symbol {
 public:
  SymbolFunction(std::vector<Tokentype> arg_type, Tokentype returnType);
  std::vector<Tokentype> argType;
  Tokentype returnType;
  std::shared_ptr<SymbolTable> funSymbolTable;
  SymbolFunction();
  bool operator==(const SymbolFunction &other) const;
};

#endif //COMPILER_COMPILER_INCLUDE_SYMBOL_H_
