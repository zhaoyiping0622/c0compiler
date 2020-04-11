//
// Created by zhaoyiping on 2020/3/31.
//

#ifndef COMPILER_COMPILER_INCLUDE_SYMBOL_H_
#define COMPILER_COMPILER_INCLUDE_SYMBOL_H_

#include "base.h"

class Symbol {
 public:
  Tokentype tokentype;
  Symbol(Tokentype tokentype);
  Symbol();
};

#endif //COMPILER_COMPILER_INCLUDE_SYMBOL_H_
