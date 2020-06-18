//
// Created by zhaoyiping on 2020/6/9.
//
#include "LifeTimeRegisterAllocator.h"
#include "Block.h"
#include "algorithm"
using namespace MIPS;

LifeTimeRegisterAllocator::LifeTimeRegisterAllocator(TAClist code) : RegisterAllocator(code) {
  blocks = Blocks(code, true);
  for (auto block:blocks.blocks)block->addLabel();
  nowBlock = 0;
  nowCode = blocks[nowBlock]->codes.begin();
  inFuntion = false;
  callArgCnt = 0;
}
Register LifeTimeRegisterAllocator::putValue2NewRegister(address value, AssemblyCodes &assemblyCodes, bool write) {
  Register physicalRegister;
  if (value2Register.count(value)) {
    physicalRegister = value2Register[value];
    if (register2Values[physicalRegister].size() > 1 && write) {
      // change to a new register
      register2Values[physicalRegister].erase(value);
      value2Register.erase(value);
    } else {
      if (write && changed.count(value) == 0)
        changed.insert(value);
      return value2Register[value];
    }
  }
  if (value == "0")return "$0";
  physicalRegister = getNewRegister(assemblyCodes);
  if (isAddress(value)) {
    address addr = getAddress(value);
    if (isInt(value)) {
      assemblyCodes.push_back(lw(physicalRegister, addr));
    } else if (isChar(value)) {
      assemblyCodes.push_back(lbu(physicalRegister, addr));
    } else
      unreachable();
    if (write)changed.insert(value);
    value2Register[value] = physicalRegister;
  } else if (isString(value))
    unreachable();
  else {
    // immediate
    assemblyCodes.push_back(li(physicalRegister, value));
    if (write)unreachable();
    value2Register[value] = physicalRegister;
  }
  return physicalRegister;
}
Register LifeTimeRegisterAllocator::putAddress2NewRegister(address addr, AssemblyCodes &assemblyCodes) {
  if (address2Register.count(addr))return address2Register[addr];
  Register physicalRegister = getNewRegister(assemblyCodes);
  if (isString(addr) || isAddress(addr))
    assemblyCodes.push_back(la(physicalRegister, getAddress(addr)));
  else
    unreachable();
  address2Register[addr] = physicalRegister;
  return physicalRegister;
}
void LifeTimeRegisterAllocator::putRegister2Address(address addr,
                                                    Register physicalRegister,
                                                    AssemblyCodes &assemblyCodes) {
  if (value2Register.count(addr))
    releaseRegister(addr, assemblyCodes, false);
  if (register2Values.count(physicalRegister) == 0)
    unreachable();
  register2Values[physicalRegister].insert(addr);
  value2Register[addr] = physicalRegister;
}
void LifeTimeRegisterAllocator::beforeTAC(AssemblyCodes &assemblyCodes) {
  // ret
  if (nowCode->op == TACRET)buildStackFrameEnd(assemblyCodes);
  TAC tac = *nowCode;
  // add sub mul div
  // mov
  // getArr setArr
  // jump
  // setRet getRet
  // setArg
  if (tac.op == TACSETARG) {
    if (callArgCnt == 0)
      storeCallerRegister(assemblyCodes);
    callArgCnt++;
  }
  // getArg
  // read
  // write
  // label
  if (tac.op == TACLABEL && isFunctionLabel(tac.ad3))enterFunction();
  // declareArg
  if (tac.op == TACDECLAREINTARG || tac.op == TACDECLARECHARARG) {
    argCnt++;
    stackFrame[tac.ad3] = (2 + argCnt) * 4;
  }
  // declareArray
  if (tac.op == TACDECLAREARRAYCHAR || tac.op == TACDECLAREARRAYINT) {
    localArrayValues[tac.ad3] = atoi(tac.ad1.c_str());
  }
  // call
  if (tac.op == TACCALL) {
    callArgCnt = 0;
  }
}
void LifeTimeRegisterAllocator::afterTAC(AssemblyCodes &assemblyCodes) {
  TAC tac = *nowCode;
  if (tac.op == TACLABEL && isFunctionLabel(tac.ad3))
    buildStackFrameBegin(assemblyCodes);
  // ret
  if (tac.op == TACRET)leaveFunction();
  // add sub mul div
  // mov
  // getArr setArr
  // jump
  // setRet getRet
  // setArg
  // getArg
  // read
  // write
  // label
  // declare
  // TODO
  // call
  if (tac.op == TACCALL) {
    loadCallerRegister(assemblyCodes);
  }
#define removeFromLifeTime(num)\
  if (lifeTime.count(tac.ad##num) && lifeTime[tac.ad1].size() && lifeTime[tac.ad1].back() == codeNumber)\
    lifeTime[tac.ad##num].pop_back();
  removeFromLifeTime(1);
  removeFromLifeTime(2);
  removeFromLifeTime(3);
  nowCode++;
  codeNumber++;
  if (nowCode == blocks[nowBlock]->codes.end()) {
    leaveBlock(assemblyCodes);
    if (nowBlock < blocks.blocks.size())
      enterBlock();
  }
}
void LifeTimeRegisterAllocator::getLifeTime() {
  lifeTime.clear();
  int num = (int)lifeTime.size() - 1;
  for (auto rit = blocks[nowBlock]->codes.rbegin(); rit != blocks[nowBlock]->codes.rend(); rit++, num--) {
    if (isRead(*rit) || (isWrite(*rit) && rit->op != TACWRITESTRING)) {
      lifeTime[rit->ad3].push_back(num);
    }
    if (isJump(*rit) && rit->op != TACJ) {
      lifeTime[rit->ad1].push_back(num);
      lifeTime[rit->ad2].push_back(num);
      continue;
    }
    switch (rit->op) {
      case TACADD:
      case TACSUB:
      case TACMUL:
      case TACDIV:
      case TACGETARR:
      case TACSETARR:lifeTime[rit->ad1].push_back(num);
        lifeTime[rit->ad2].push_back(num);
        lifeTime[rit->ad3].push_back(num);
        continue;
      case TACMOV:lifeTime[rit->ad1].push_back(num);
        lifeTime[rit->ad3].push_back(num);
        continue;
      case TACSETARG:
      case TACGETARG:
      case TACSETRET:
      case TACGETRET:lifeTime[rit->ad3].push_back(num);
        continue;
    }
  }
}
/*
 * frame:
 * | caller saved registers   | bsp
 * | argN                     |
 * | argN-1                   |
 * | ...                      |
 * | arg1                     |
 * | bra                      |
 * | bfp                      |
 * | bsp                      | fp
 * | values                   |
 * | ...                      |
 * | values                   |
 * | callee saved registers   |
 * | ...                      |
 * | callee saved registers   | sp
 * | caller saved registers   |
 * | ...                      |
 * | caller saved registers   | sp (change sp while call function)
 */
void LifeTimeRegisterAllocator::buildStackFrameBegin(AssemblyCodes &assemblyCodes) {
  assemblyCodes.push_back(comments(BUILDSTACK));
}
void LifeTimeRegisterAllocator::buildStackFrameEnd(AssemblyCodes &assemblyCodes) {
  for (auto rit = assemblyCodes.rbegin(); rit != assemblyCodes.rend(); rit++)
    if (*rit == comments(BUILDSTACK)) {
      auto it = rit.base();
      // fp=bsp-argcnt*4-8
      AssemblyCodes codes;
      // save registers
      codes.push_back(sw(ra, getLocation(sp, -(argCnt + 1) * 4)));
      codes.push_back(sw(fp, getLocation(sp, -(argCnt + 2) * 4)));
      codes.push_back(sw(sp, getLocation(sp, -(argCnt + 3) * 4)));
      // change fp
      codes.push_back(subu(fp, fp, (argCnt + 3) * 4));
      // change sp
      codes.push_back(subu(sp, sp, (argCnt + 3 + (int)calleeRegisterUsed.size()) * 4 + spaceUsed));
      // save callee saved registers
      auto it1 = calleeRegisterUsed.begin();
      for (int i = 0; i < calleeRegisterUsed.size(); i++, it1++)
        codes.push_back(sw(*it1, getLocation(sp, i * 4)));
      // save arg 1-4
      for (int i = 0; i < 4 && i < argCnt; i++)
        codes.push_back(sw("$a" + std::to_string(i), getLocation(fp, 12 + i * 4)));
      assemblyCodes.splice(it, codes);
      assemblyCodes.erase(it);
      break;
    }
  assemblyCodes.push_back(lw(sp, getLocation(fp, 0)));
  assemblyCodes.push_back(lw(ra, getLocation(fp, 8)));
  assemblyCodes.push_back(lw(fp, getLocation(fp, 4)));
}
int LifeTimeRegisterAllocator::getOffset(address addr) {
  if (stackFrame.count(addr))return stackFrame[addr];
  // int -4 -8 -12
  // char -1 -2 -3 -4
  if (isInt(addr)) {
    if (localArrayValues.count(addr))
      spaceUsed -= localArrayValues[addr] * 4;
    else
      spaceUsed -= 4;
    stackFrame[addr] = spaceUsed;
  } else if (isChar(addr)) {
    int num = localArrayValues.count(addr) ? localArrayValues[addr] : 1;
    int space = (num + 3) / 4 * 4;
    if (charLocation - space < spaceUsed) {
      spaceUsed -= space;
      charLocation = spaceUsed + 4;
    }
    charLocation -= num;
    stackFrame[addr] = charLocation;
  } else unreachable();
  return stackFrame[addr];
}
address LifeTimeRegisterAllocator::getStringName(address string) {
  if (!stringName.count(string))
    stringName[string] = "s" + std::to_string(stringName.size() + 1);
  return stringName[string];
}
void LifeTimeRegisterAllocator::enterFunction() {
  stackFrame.clear();
  localArrayValues.clear();
  argCnt = 0;
  calleeRegisterUsed.clear();
  inFuntion = true;
  charLocation = spaceUsed = 0;
  callArgCnt = 0;
  for (int i = 0; i < 10; i++)unusedCallerRegisters.insert("$t" + std::to_string(i));
  for (int i = 0; i < 8; i++)unusedCalleeRegisters.insert("$s" + std::to_string(i));
}
void LifeTimeRegisterAllocator::leaveFunction() {
  inFuntion = false;
}
LifeTimeRegisterAllocator::~LifeTimeRegisterAllocator() {}
void LifeTimeRegisterAllocator::leaveBlock(AssemblyCodes &assemblyCodes) {
  for (auto addr:changed) {
    if (blocks[nowBlock]->LiveOut.count(addr) || isGlobal(addr)) {
      // if addr may be used later
      if (isInt(addr))
        releaseRegister(addr, assemblyCodes);
      else if (isChar(addr))
        releaseRegister(addr, assemblyCodes);
    }
  }
  nowBlock++;
}
void LifeTimeRegisterAllocator::enterBlock() {
  nowCode = blocks[nowBlock]->codes.begin();
  lifeTime.clear();
  value2Register.clear();
  register2Values.clear();
  address2Register.clear();
  changed.clear();
  for (int i = 0; i < 10; i++)unusedCallerRegisters.insert("$t" + std::to_string(i));
  for (int i = 0; i < 8; i++)unusedCalleeRegisters.insert("$s" + std::to_string(i));
  getLifeTime();
  codeNumber = 0;
}
void LifeTimeRegisterAllocator::storeCallerRegister(AssemblyCodes &assemblyCodes) {
  // TODO: some global value need to be release
  for (auto reg:allCallerSavedRegister) {
    if (unusedCallerRegisters.count(reg) == 0) {
      callerRegisters[reg] = (int)callerRegisters.size() + 1;
      assemblyCodes.push_back(sw(reg, getLocation(sp, -4 * callerRegisters[reg])));
    }
  }
  assemblyCodes.push_back(subu(sp, sp, (int)callerRegisters.size() * 4)
  );
}
void LifeTimeRegisterAllocator::loadCallerRegister(AssemblyCodes &assemblyCodes) {
  assemblyCodes.push_back(addu(sp, sp, (int)callerRegisters.size() * 4));
  for (auto[k, v]:callerRegisters) {
    assemblyCodes.push_back(lw(k, getLocation(sp, -4 * v)));
  }
  callerRegisters.clear();
}
Register LifeTimeRegisterAllocator::getNewRegister(AssemblyCodes &assemblyCodes) {
  // TODO: find a unused register first
  auto cmp = [&](Register register1, Register register2) {
    auto getMin = [&](Register reg) {
      int value1 = (int)blocks[nowBlock]->codes.size();
      std::vector<address> remove;
      for (auto addr:register2Values[reg]) {
        if (lifeTime.count(addr) && lifeTime[addr].size())
          value1 = std::min(lifeTime[addr].back(), value1);
        else
          remove.push_back(addr);
      }
      for (auto addr:remove) {
        register2Values[reg].erase(addr);
      }
      return value1;
    };
    if (register2Values[register1].empty())return true;
    if (register2Values[register2].empty())return false;
    return getMin(register1) - register2Values[register1].size()
        < getMin(register2) - register2Values[register2].size();
  };
  if (unusedCallerRegisters.size()) {
    Register ret = *unusedCallerRegisters.begin();
    unusedCallerRegisters.erase(ret);
    return ret;
  }
  if (unusedCalleeRegisters.size()) {
    Register ret = *unusedCalleeRegisters.begin();
    unusedCalleeRegisters.erase(ret);
    calleeRegisterUsed.insert(ret);
    return ret;
  }
  auto reg1 = *std::max_element(allCallerSavedRegister.begin(), allCallerSavedRegister.end());
  auto reg2 = *std::max_element(allCalleeSavedRegister.begin(), allCalleeSavedRegister.end());
  Register reg;
  if (cmp(reg1, reg2))reg = reg2;
  else reg = reg1;
  for (auto addr:register2Values[reg]) {
    releaseRegister(addr, assemblyCodes, blocks[nowBlock]->LiveOut.count(addr) == 1);
  }
  register2Values[reg].clear();
  return reg;
}
address LifeTimeRegisterAllocator::getAddress(address addr) {
  if (isString(addr))return getStringName(addr);
  if (isGlobal(addr))return addr;
  return getLocation(fp, getOffset(addr));
}
Register LifeTimeRegisterAllocator::releaseRegister(address value, AssemblyCodes &assemblyCodes, bool saveChange) {
  Register restoreRegister;
  if (address2Register.count(value)) {
    restoreRegister = address2Register[value];
  } else if (value2Register.count(value)) {
    restoreRegister = value2Register[value];
  }
  if (saveChange && changed.count(value)) {
    if (isInt(value))
      assemblyCodes.push_back(sw(restoreRegister, getAddress(value)));
    else if (isChar(value))
      assemblyCodes.push_back(sb(restoreRegister, getAddress(value)));
    changed.erase(value);
  }
  if (isCallerSavedRegister(restoreRegister))
    unusedCallerRegisters.insert(restoreRegister);
  else if (isCalleeSavedRegister(restoreRegister))
    unusedCalleeRegisters.insert(restoreRegister);
  else unreachable();
  register2Values[restoreRegister].erase(value);
  return restoreRegister;
}
LifeTimeRegisterAllocator::LifeTimeRegisterAllocator() {}

