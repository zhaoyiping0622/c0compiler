//
// Created by zhaoyiping on 2020/5/16.
//

#ifndef COMPILER_COMPILER_INCLUDE_MIPSOPTIMIZER_H_
#define COMPILER_COMPILER_INCLUDE_MIPSOPTIMIZER_H_

#include "MIPS.h"

class MIPSCode {
 public:
  std::string op;
  std::string var1;
  std::string var2;
  std::string var3;
};

class MIPSOptimizer {
 public:
  virtual void optimize(AssemblyCodes &codes) = 0;
};

class Peephole : public MIPSOptimizer {
 public:
  void optimize(AssemblyCodes &codes);
  bool loadAndStore(AssemblyCodes &codes, AssemblyCodes::iterator it);
  bool afterJump(AssemblyCodes &codes, AssemblyCodes::iterator it);
  bool control(AssemblyCodes &codes, AssemblyCodes::iterator it);
  bool branch(AssemblyCodes &codes,AssemblyCodes::iterator it);
};

#endif //COMPILER_COMPILER_INCLUDE_MIPSOPTIMIZER_H_
