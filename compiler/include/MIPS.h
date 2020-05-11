//
// Created by zhaoyiping on 2020/5/5.
//

#ifndef COMPILER_COMPILER_INCLUDE_MIPS_H_
#define COMPILER_COMPILER_INCLUDE_MIPS_H_

#include "base.h"
#include "memory"
#include "stack"
#include "unordered_set"
#include "generator.h"
#include "RegisterAllocator.h"

namespace MIPS {
extern const Register sp;
extern const Register fp;
extern const Register ra;
address getLocation(Register reg, int offset);
address getLocation(Register reg, address offset);
// load
// load address
AssemblyCode la(Register dst, address src);
// load unsigned byte
AssemblyCode lbu(Register dst, address src);
// load word
AssemblyCode lw(Register dst, address src);
// load immediate
AssemblyCode li(Register dst, address src);
// store
// store byte
AssemblyCode sb(Register src, address dst);
// store word
AssemblyCode sw(Register src, address dst);
// compute
// add unsigned
AssemblyCode addu(Register dst, Register src1, Register src2);
AssemblyCode addu(Register dst, Register src, int immediate);
// sub unsigned
AssemblyCode subu(Register dst, Register src1, Register src2);
AssemblyCode subu(Register dst, Register src, int immediate);
// div signed
AssemblyCode div(Register dst, Register src1, Register src2);
AssemblyCode div(Register dst, Register src, int immediate);
// mul signed
AssemblyCode mul(Register dst, Register src1, Register src2);
AssemblyCode mul(Register dst, Register src, int immediate);
// move
AssemblyCode move(Register dst, Register src);
// shift left logical
AssemblyCode sll(Register dst, Register src1, Register src2);
AssemblyCode sll(Register dst, Register src, int immediate);
// shift left Arithmetic
AssemblyCode sra(Register dst, Register src1, Register src2);
AssemblyCode sra(Register dst, Register src, int immediate);
// jump and branch
AssemblyCode j(address addr);
AssemblyCode jr(address addr);
AssemblyCode jal(address addr);
// branch on equal
AssemblyCode beq(Register src1, Register src2, address addr);
AssemblyCode beq(Register src1, int immediate, address addr);
// branch on not equal
AssemblyCode bne(Register src1, Register src2, address addr);
AssemblyCode bne(Register src1, int immediate, address addr);
// branch on greater
AssemblyCode bgt(Register src1, Register src2, address addr);
AssemblyCode bgt(Register src1, int immediate, address addr);
// branch on greater/equal
AssemblyCode bge(Register src1, Register src2, address addr);
AssemblyCode bge(Register src1, int immediate, address addr);
// branch on less
AssemblyCode blt(Register src1, Register src2, address addr);
AssemblyCode blt(Register src1, int immediate, address addr);
// branch on less/equal
AssemblyCode ble(Register src1, Register src2, address addr);
AssemblyCode ble(Register src1, int immediate, address addr);
// label
AssemblyCode setLabel(address addr);
// syscall
AssemblyCode syscall();
/* stack frame
 * the first 4 arg is stored in a0-a4 which should be stored in stack
 * sp-before(bsp)->| restore registerN |
 *                 | ....              |
 *                 | arg4              |
 *                 | arg3              |
 *                 | arg2              | +($fp)
 * fp------------->| arg1              | 0($fp)
 *                 | declare value1    | -($fp)
 *                 | declare value2    |
 *                 | declare value3    |
 *                 | ..............    |
 *                 | declare valueN    |
 *                 | tmp value1        |
 *                 | tmp value2        |
 *                 | tmp value3        |
 *                 | ..........        |
 *                 | tmp valueN        |
 *                 | bra               |
 *                 | bsp               |
 *                 | bfp               | upper build by this function
 *                 | restore register1 |
 *                 | restore register2 |
 *                 | restore register3 |
 *                 | ................. |
 * sp------------->| restore registerN |
 *
 * begin: the iterator to the begin of taclist
 * end: the end of taclist
 * return: map from value name to offset
 * args/declare value/tmp value
 */
std::unordered_map<address, int> getStackFrame(TAClist::iterator begin, TAClist::const_iterator end);
};

using namespace MIPS;
class DefaultMIPSRegisterAllocator : public RegisterAllocator {
 public:
  DefaultMIPSRegisterAllocator();
  DefaultMIPSRegisterAllocator(TAClist code);
  Register putValue2NewRegister(address value, AssemblyCodes &assemblyCodes, bool write) override;
  Register putAddress2NewRegister(address value, AssemblyCodes &assemblyCodes, bool write) override;
  Register putValue2Location(address value, Register physicalRegister, AssemblyCodes &assemblyCodes) override;
  void beforeTAC(AssemblyCodes &assemblyCodes);
  void afterTAC(AssemblyCodes &assemblyCodes);
 protected:
  AssemblyCodes data;
  TAClist::iterator now;
  std::unordered_map<address, address> stringName;
  address getAddress(address value);
  void buildStackFrameBegin(AssemblyCodes &assemblyCodes);
  void buildStackFrameEnd(AssemblyCodes &assemblyCodes);
  address getStringName(address string);
  std::unordered_map<Register, std::pair<int, address>> restoreRegisters;
  std::unordered_set<Register> usedRegisters;
  Register getNewRegister();
  std::unordered_map<address, int> stackFrame;
};

template<typename RegisterAllocator_=DefaultMIPSRegisterAllocator>
class MIPSGenerator : public Generator {
 public:
  MIPSGenerator() {
    inFunction = false;
  }
  AssemblyCodes translate(TAClist tacList) {
    registerAllocator = std::make_unique<RegisterAllocator_>(tacList);
    AssemblyCodes thisFunction;
    text.push_back(".text");
    text.push_back(jal("main"));
    text.push_back(li("$v0", "10"));
    text.push_back(syscall());
    for (auto it = tacList.begin(); it != tacList.end(); it++) {
      auto tac = *it;
      if (tac.op == TACLABEL && tac.ad3.find_first_of("label") != 0) {
        // the begin of a function
        inFunction = true;
      }
      registerAllocator->beforeTAC(thisFunction);
      thisFunction.splice(thisFunction.end(), translate(tac));
      registerAllocator->afterTAC(thisFunction);
      if (tac.op == TACRET) {
        // the end of a function
        text.splice(text.end(), thisFunction);
        thisFunction.clear();
        inFunction = false;
      }
      tmp.clear();
    }
    if (data.size()) {
      data.push_front(".data");
      text.splice(text.end(), data);
    }
    return text;
  }
 protected:
  AssemblyCodes text;
  AssemblyCodes data;
  bool inFunction;
  AssemblyCodes translate(TAC tac) {
    switch (tac.op) {
      case TACADD: return translateAdd(tac);
      case TACSUB: return translateSub(tac);
      case TACMUL: return translateMul(tac);
      case TACDIV: return translateDiv(tac);
      case TACMOV: return translateMov(tac);
      case TACGETARR: return translateGetArr(tac);
      case TACSETARR: return translateSetArr(tac);
      case TACJLE: return translateJle(tac);
      case TACJGE: return translateJge(tac);
      case TACJNE: return translateJne(tac);
      case TACJEQ: return translateJeq(tac);
      case TACJG: return translateJg(tac);
      case TACJL: return translateJl(tac);
      case TACCALL: return translateCall(tac);
      case TACRET: return translateRet(tac);
      case TACSETRET: return translateSetRet(tac);
      case TACGETRET: return translateGetRet(tac);
      case TACSETARG: return translateSetArg(tac);
      case TACGETARG: return translateGetArg(tac);
      case TACJ: return translateJ(tac);
      case TACREADINT: return translateReadInt(tac);
      case TACREADCHAR: return translateReadChar(tac);
      case TACWRITEINT: return translateWriteInt(tac);
      case TACWRITECHAR: return translateWriteChar(tac);
      case TACWRITESTRING: return translateWriteString(tac);
      case TACDECLAREINTARG:
      case TACDECLARECHARARG: return tmp;
      case TACDECLAREINT:
      case TACDECLARECHAR:
      case TACDECLARESTRING:
      case TACDECLAREARRAYINT:
      case TACDECLAREARRAYCHAR:if (!inFunction || tac.op == TACDECLARESTRING)initData(tac);
        return tmp;
      case TACLABEL: return translateLabel(tac);
      default: unreachable();
    }
  }
  AssemblyCodes tmp;
  std::unique_ptr<RegisterAllocator> registerAllocator;
  AssemblyCodes translateAdd(TAC code) {
    if (!isAddress(code.ad1))swap(code.ad1, code.ad2);
    Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
    Register physicalRegister3 = registerAllocator->putValue2NewRegister(code.ad3, tmp, true);
    if (isAddress(code.ad2))
      tmp.push_back(addu(physicalRegister3, physicalRegister1,
                         registerAllocator->putValue2NewRegister(code.ad2, tmp, false)));
    else
      tmp.push_back(addu(physicalRegister3, physicalRegister1, atoi(code.ad2.c_str())));
    return tmp;
  }
  AssemblyCodes translateSub(TAC code) {
    Register physicalRegister3 = registerAllocator->putValue2NewRegister(code.ad3, tmp, true);
    Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
    if (isAddress(code.ad2))
      tmp.push_back(subu(physicalRegister3, physicalRegister1,
                         registerAllocator->putValue2NewRegister(code.ad2, tmp, false)));
    else
      tmp.push_back(subu(physicalRegister3, physicalRegister1, atoi(code.ad2.c_str())));
    return tmp;
  }
  AssemblyCodes translateMul(TAC code) {
    if (!isAddress(code.ad1))swap(code.ad1, code.ad2);
    Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
    Register physicalRegister3 = registerAllocator->putValue2NewRegister(code.ad3, tmp, true);
    if (isAddress(code.ad2))
      tmp.push_back(mul(physicalRegister3, physicalRegister1,
                        registerAllocator->putValue2NewRegister(code.ad2, tmp, false)));
    else
      tmp.push_back(mul(physicalRegister3, physicalRegister1, atoi(code.ad2.c_str())));
    return tmp;
  }
  AssemblyCodes translateDiv(TAC code) {
    Register physicalRegister3 = registerAllocator->putValue2NewRegister(code.ad3, tmp, true);
    Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
    if (isAddress(code.ad2))
      tmp.push_back(div(physicalRegister3, physicalRegister1,
                        registerAllocator->putValue2NewRegister(code.ad2, tmp, false)));
    else
      tmp.push_back(div(physicalRegister3, physicalRegister1, atoi(code.ad2.c_str())));
    return tmp;
  }
  AssemblyCodes translateMov(TAC code) {
    Register physicalRegister3 = registerAllocator->putValue2NewRegister(code.ad3, tmp, true);
    if (isAddress(code.ad1))
      tmp.push_back(move(physicalRegister3, registerAllocator->putValue2NewRegister(code.ad1, tmp, false)));
    else
      tmp.push_back(li(physicalRegister3, code.ad1));
    return tmp;
  }
  AssemblyCodes translateGetArr(TAC code) {
    Register physicalRegister3 = registerAllocator->putValue2NewRegister(code.ad3, tmp, true);
    Register physicalRegister1 = registerAllocator->putAddress2NewRegister(code.ad1, tmp, false);
    if (isAddress(code.ad2)) {
      Register physicalRegister2 = registerAllocator->putValue2NewRegister(code.ad2, tmp, false);
      if (isInt(code.ad1))
        tmp.push_back(sll(physicalRegister2, physicalRegister2, 2));
      tmp.push_back(addu(physicalRegister1, physicalRegister1, physicalRegister2));
      tmp.push_back(lw(physicalRegister3, getLocation(physicalRegister1, 0)));
    } else {
      int offset = atoi(code.ad2.c_str());
      if (isInt(code.ad1))offset <<= 2;
      tmp.push_back(lw(physicalRegister3, getLocation(physicalRegister1, offset)));
    }
    return tmp;
  }
  AssemblyCodes translateSetArr(TAC code) {
    Register physicalRegister3 = registerAllocator->putValue2NewRegister(code.ad3, tmp, false);
    Register physicalRegister1 = registerAllocator->putAddress2NewRegister(code.ad1, tmp, false);
    if (isAddress(code.ad2)) {
      Register physicalRegister2 = registerAllocator->putValue2NewRegister(code.ad2, tmp, false);
      if (isInt(code.ad1))
        tmp.push_back(sll(physicalRegister2, physicalRegister2, 2));
      tmp.push_back(addu(physicalRegister1, physicalRegister1, physicalRegister2));
      tmp.push_back(sw(physicalRegister3, getLocation(physicalRegister1, 0)));
    } else {
      int offset = atoi(code.ad2.c_str());
      if (isInt(code.ad1))offset <<= 2;
      tmp.push_back(sw(physicalRegister3, getLocation(physicalRegister1, offset)));
    }
    return tmp;
  }
  AssemblyCodes translateJle(TAC code) {
    if (isAddress(code.ad1)) {
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(ble(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(ble(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    } else {
      swap(code.ad1, code.ad2);
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(bge(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(bge(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    }
    return tmp;
  }
  AssemblyCodes translateJge(TAC code) {
    if (isAddress(code.ad1)) {
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(bge(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(bge(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    } else {
      swap(code.ad1, code.ad2);
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(ble(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(ble(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    }
    return tmp;
  }
  AssemblyCodes translateJne(TAC code) {
    if (isAddress(code.ad1)) {
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(bne(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(bne(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    } else {
      swap(code.ad1, code.ad2);
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(bne(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(bne(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    }
    return tmp;
  }
  AssemblyCodes translateJeq(TAC code) {
    if (isAddress(code.ad1)) {
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(beq(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(beq(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    } else {
      swap(code.ad1, code.ad2);
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(beq(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(beq(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    }
    return tmp;
  }
  AssemblyCodes translateJl(TAC code) {
    if (isAddress(code.ad1)) {
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(blt(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(blt(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    } else {
      swap(code.ad1, code.ad2);
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(bgt(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(bgt(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    }
    return tmp;
  }
  AssemblyCodes translateJg(TAC code) {
    if (isAddress(code.ad1)) {
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(bgt(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(bgt(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    } else {
      swap(code.ad1, code.ad2);
      Register physicalRegister1 = registerAllocator->putValue2NewRegister(code.ad1, tmp, false);
      if (isAddress(code.ad2))
        tmp.push_back(blt(physicalRegister1, registerAllocator->putValue2NewRegister(code.ad2, tmp, false), code.ad3));
      else
        tmp.push_back(blt(physicalRegister1, atoi(code.ad2.c_str()), code.ad3));
    }
    return tmp;
  }
  int argLen = 0;
  AssemblyCodes translateCall(TAC code) {
    argLen = 0;
    tmp.push_back(jal(code.ad3));
    return tmp;
  }
  AssemblyCodes translateRet(TAC code) {
    tmp.push_back(jr(ra));
    return tmp;
  }
  AssemblyCodes translateSetRet(TAC code) {
    if (isAddress(code.ad3))
      tmp.push_back(move("$v0", registerAllocator->putValue2NewRegister(code.ad3, tmp, false)));
    else
      tmp.push_back(li("$v0", code.ad3));
    return tmp;
  }
  AssemblyCodes translateGetRet(TAC code) {
    Register physicalRegister3 = registerAllocator->putValue2NewRegister(code.ad3, tmp, true);
    tmp.push_back(move(physicalRegister3, "$v0"));
    return tmp;
  }
  AssemblyCodes translateSetArg(TAC code) {
    int arg = atoi(code.ad1.c_str());
    if (arg <= 4) {
      if (isAddress(code.ad3))
        tmp.push_back(move("$a" + std::to_string(arg - 1),
                           registerAllocator->putValue2NewRegister(code.ad3, tmp, false)));
      else tmp.push_back(li("$a" + std::to_string(arg - 1), code.ad3));
    } else {
      argLen -= 4;
      Register physicalRegister = registerAllocator->putValue2NewRegister(code.ad3, tmp, false);
      tmp.push_back(sw(physicalRegister, getLocation(sp, argLen)));
    }
    return tmp;
  }
  AssemblyCodes translateGetArg(TAC code) {
    int offset = atoi(code.ad1.c_str()) * 4 - 4;
    Register physicalRegister = registerAllocator->putValue2NewRegister(code.ad3, tmp, true);
    tmp.push_back(lw(physicalRegister, getLocation(fp, offset)));
    return tmp;
  }
  AssemblyCodes translateJ(TAC code) {
    tmp.push_back(j(code.ad3));
    return tmp;
  }
  AssemblyCodes translateReadInt(TAC code) {
    tmp.push_back(li("$v0", "5"));
    tmp.push_back(syscall());
    registerAllocator->putValue2Location(code.ad3, "$v0", tmp);
    return tmp;
  }
  AssemblyCodes translateReadChar(TAC code) {
    tmp.push_back(li("$v0", "12"));
    tmp.push_back(syscall());
    registerAllocator->putValue2Location(code.ad3, "$v0", tmp);
    return tmp;
  }
  AssemblyCodes translateWriteInt(TAC code) {
    if (isAddress(code.ad3)) {
      Register physicalRegister = registerAllocator->putValue2NewRegister(code.ad3, tmp, false);
      tmp.push_back(move("$a0", physicalRegister));
    } else {
      tmp.push_back(li("$a0", code.ad3));
    }
    tmp.push_back(li("$v0", "1"));
    tmp.push_back(syscall());
    return tmp;
  }
  AssemblyCodes translateWriteChar(TAC code) {
    if (isAddress(code.ad3)) {
      Register physicalRegister = registerAllocator->putValue2NewRegister(code.ad3, tmp, false);
      tmp.push_back(move("$a0", physicalRegister));
    } else {
      tmp.push_back(li("$a0", code.ad3));
    }
    tmp.push_back(li("$v0", "11"));
    tmp.push_back(syscall());
    return tmp;
  }
  AssemblyCodes translateWriteString(TAC code) {
    Register physicalRegister = registerAllocator->putAddress2NewRegister(code.ad3, tmp, false);
    tmp.push_back(move("$a0", physicalRegister));
    tmp.push_back(li("$v0", "4"));
    tmp.push_back(syscall());
    return tmp;
  }
  AssemblyCodes translateLabel(TAC code) {
    tmp.push_back(setLabel(code.ad3));
    return tmp;
  }
  std::unordered_map<address, address> stringName;
  address getStringName(address string) {
    if (!stringName.count(string))
      stringName[string] = "s" + std::to_string(stringName.size() + 1);
    return stringName[string];
  }
  void initData(TAC tac) {
    switch (tac.op) {
      case TACDECLAREINT:data.push_back(tac.ad3 + ": .word 0");
        break;
      case TACDECLARECHAR:data.push_back(tac.ad3 + ": .byte 0");
        break;
      case TACDECLAREARRAYINT:data.push_back(tac.ad3 + ": .space " + std::to_string(atoi(tac.ad1.c_str()) * 4));
        break;
      case TACDECLAREARRAYCHAR:data.push_back(tac.ad3 + ": .space " + tac.ad1);
        break;
      case TACDECLARESTRING:
        data.push_back(
            getStringName(tac.ad3) + ": .ascii " + tac.ad3.substr(0, tac.ad3.size() - 1) + "\\0" + tac.ad3.back());
        break;
    }
  }
};

#endif //COMPILER_COMPILER_INCLUDE_MIPS_H_
