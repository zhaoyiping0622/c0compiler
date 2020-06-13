//
// Created by zhaoyiping on 2020/6/9.
//

#ifndef COMPILER_COMPILER_INCLUDE_LIFETIMEREGISTERALLOCATOR_H_
#define COMPILER_COMPILER_INCLUDE_LIFETIMEREGISTERALLOCATOR_H_
#include "RegisterAllocator.h"
#include "Block.h"
#include "MIPS.h"
#include "queue"

/*
 * 1. determine whether store value by lifetime check
 * 2. many address share one register if their value are not changed
 * 3. add comments may remove at end
 */
class LifeTimeRegisterAllocator : RegisterAllocator {
 public:
  LifeTimeRegisterAllocator(TAClist code);
  virtual Register putValue2NewRegister(address value, AssemblyCodes &assemblyCodes, bool write) override;
  virtual Register putAddress2NewRegister(address addr, AssemblyCodes &assemblyCodes) override;
  virtual void putRegister2Address(address addr, Register physicalRegister, AssemblyCodes &assemblyCodes) override;
  virtual void beforeTAC(AssemblyCodes &assemblyCodes) override;
  virtual void afterTAC(AssemblyCodes &assemblyCodes) override;
  virtual ~LifeTimeRegisterAllocator();
 protected:
  const std::string BUILDSTACK = "build stack here";
  std::unordered_map<address, int> stackFrame; // begin of function -- end of function
  Blocks blocks; // init in constructor
  int nowBlock; // increase in leaveBlock
  TAClist::iterator nowCode; // increase in afterTAC
  int codeNumber;
  std::unordered_map<address, std::vector<int>> lifeTime; // begin of block -- end of block
  // may contain immediate
  std::unordered_map<address, Register> value2Register; // begin of block -- end of block
  std::unordered_map<address, Register> address2Register; // begin of block -- end of block
  std::unordered_map<Register, std::unordered_set<address>> register2Values; // begin of block -- end of block
  std::unordered_map<address, int> localArrayValues; // begin of function -- end of function
  std::unordered_set<address> changed; // begin of block -- end of block
  std::unordered_map<address, address> stringName; // not clear
  std::unordered_map<address, int> callerRegisters;  // begin of call -- end of call
  std::unordered_set<Register> unusedCallerRegisters; // remove dynamically resume at end of block
  std::unordered_set<Register> unusedCalleeRegisters; // remove dynamically resume at end of block
  int charLocation; // begin of function -- end of function
  int spaceUsed; // begin of function -- end of function
  bool inFuntion; // begin of function -- end of function
  int argCnt; // begin of function -- end of function
  int callArgCnt; // begin of function -- end of function
  std::unordered_set<Register> calleeRegisterUsed; // begin of function -- end of function
  address getStringName(address string);
  void enterFunction();
  void leaveFunction();
  void getLifeTime();
  void buildStackFrameBegin(AssemblyCodes &assemblyCodes);
  void buildStackFrameEnd(AssemblyCodes &assemblyCodes);
  int getOffset(address addr);
  void leaveBlock(AssemblyCodes &assemblyCodes);
  void enterBlock();
  void storeCallerRegister(AssemblyCodes &assemblyCodes);
  void loadCallerRegister(AssemblyCodes &assemblyCodes);
  Register getNewRegister(AssemblyCodes &assemblyCodes);
  address getAddress(address value);
  Register releaseRegister(address value, AssemblyCodes &assemblyCodes, bool saveChange = true);
};

#endif //COMPILER_COMPILER_INCLUDE_LIFETIMEREGISTERALLOCATOR_H_
