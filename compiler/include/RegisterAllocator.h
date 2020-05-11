//
// Created by zhaoyiping on 2020/5/5.
//

#ifndef COMPILER_COMPILER_INCLUDE_REGISTERALLOCATOR_H_
#define COMPILER_COMPILER_INCLUDE_REGISTERALLOCATOR_H_

#include "string"
#include "unordered_map"
#include "generator.h"

typedef std::string Register;
/*
 * allocate physical register
 * store the global value
 */
class RegisterAllocator {
 protected:
  TAClist code;
 public:
  RegisterAllocator(TAClist code) : code(code) {}
  RegisterAllocator() {}
  /*
   * get a new Register
   * return $t0-$t9 $s0-$s7
   */
  virtual Register putValue2NewRegister(address value, AssemblyCodes &assemblyCodes, bool write) = 0;
  virtual Register putAddress2NewRegister(address addr, AssemblyCodes &assemblyCodes, bool write) = 0;
  // from physicalRegister to the location of value
  virtual Register putValue2Location(address value, Register physicalRegister, AssemblyCodes &assemblyCodes) = 0;
  virtual void beforeTAC(AssemblyCodes &assemblyCodes) = 0;
  virtual void afterTAC(AssemblyCodes &assemblyCodes) = 0;
  virtual ~RegisterAllocator() {}
};

#endif //COMPILER_COMPILER_INCLUDE_REGISTERALLOCATOR_H_
