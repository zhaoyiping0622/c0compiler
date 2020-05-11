//
// Created by zhaoyiping on 2020/5/5.
//

#ifndef COMPILER_COMPILER_INCLUDE_GENERATOR_H_
#define COMPILER_COMPILER_INCLUDE_GENERATOR_H_

#include "TAC.h"

typedef std::string AssemblyCode;
typedef std::list<AssemblyCode> AssemblyCodes;

class Generator {
 public:
  virtual AssemblyCodes translate(TAClist tacList) = 0;
  Generator() {}
};

#endif //COMPILER_COMPILER_INCLUDE_GENERATOR_H_
