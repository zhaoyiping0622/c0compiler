//
// Created by zhaoyiping on 2020/5/8.
//
#include "MIPS.h"
#include "set"

namespace MIPS {
const Register sp = "$sp";
const Register fp = "$fp";
const Register ra = "$ra";
address getLocation(Register reg, int offset) {
  return std::to_string(offset) + "(" + reg + ")";
}
address getLocation(Register reg, std::string offset) {
  return offset + "(" + reg + ")";
}
#define op3r(name, r1, r2, r3) {return std::string(#name)+" "+r1+","+r2+","+r3;}
#define op2r1i(name, r1, r2, r3) {return std::string(#name)+" "+r1+","+r2+","+std::to_string(r3);}
#define op1r1i1r(name, r1, r2, r3) {return std::string(#name)+" "+r1+","+std::to_string(r2)+","+r3;}
#define op2r(name, r1, r2) {return std::string(#name)+" "+r1+","+r2;}
// load
// load address
AssemblyCode la(Register dst, address src) op2r(la, dst, src)
// load unsigned byte
AssemblyCode lbu(Register dst, address src) op2r(lbu, dst, src)
// load word
AssemblyCode lw(Register dst, address src) op2r(lw, dst, src)
// load immediate
AssemblyCode li(Register dst, address src) op2r(li, dst, src)
// store
// store byte
AssemblyCode sb(Register src, address dst) op2r(sb, src, dst)
// store word
AssemblyCode sw(Register src, address dst) op2r(sw, src, dst)
// compute
// add unsigned
AssemblyCode addu(Register dst, Register src1, Register src2) op3r(addu, dst, src1, src2)
AssemblyCode addu(Register dst, Register src, int immediate) op2r1i(addu, dst, src, immediate)
// sub unsigned
AssemblyCode subu(Register dst, Register src1, Register src2) op3r(subu, dst, src1, src2)
AssemblyCode subu(Register dst, Register src, int immediate) op2r1i(subu, dst, src, immediate)
// div signed
AssemblyCode div(Register dst, Register src1, Register src2) op3r(div, dst, src1, src2)
AssemblyCode div(Register dst, Register src, int immediate) op2r1i(div, dst, src, immediate)
// mul signed
AssemblyCode mul(Register dst, Register src1, Register src2) op3r(mul, dst, src1, src2)
AssemblyCode mul(Register dst, Register src, int immediate) op2r1i(mul, dst, src, immediate)
// move
AssemblyCode move(Register dst, Register src) op2r(move, dst, src)
// shift left logical
AssemblyCode sll(Register dst, Register src1, Register src2) op3r(sll, dst, src1, src2)
AssemblyCode sll(Register dst, Register src, int immediate) op2r1i(sll, dst, src, immediate)
// shift left Arithmetic
AssemblyCode sra(Register dst, Register src1, Register src2) op3r(sra, dst, src1, src2)
AssemblyCode sra(Register dst, Register src, int immediate) op2r1i(sra, dst, src, immediate)
// jump and branch
AssemblyCode j(address addr) { return "j " + addr; }
AssemblyCode jr(address addr) { return "jr " + addr; }
AssemblyCode jal(address addr) { return "jal " + addr; }
// branch on equal
AssemblyCode beq(Register src1, Register src2, address addr) op3r(beq, src1, src2, addr)
AssemblyCode beq(Register src1, int immediate, address addr) op1r1i1r(beq, src1, immediate, addr)
// branch on not equal
AssemblyCode bne(Register src1, Register src2, address addr) op3r(bne, src1, src2, addr)
AssemblyCode bne(Register src1, int immediate, address addr) op1r1i1r(bne, src1, immediate, addr)
// branch on greater
AssemblyCode bgt(Register src1, Register src2, address addr) op3r(bgt, src1, src2, addr)
AssemblyCode bgt(Register src1, int immediate, address addr) op1r1i1r(bgt, src1, immediate, addr)
// branch on greater/equal
AssemblyCode bge(Register src1, Register src2, address addr) op3r(bge, src1, src2, addr)
AssemblyCode bge(Register src1, int immediate, address addr) op1r1i1r(bge, src1, immediate, addr)
// branch on less
AssemblyCode blt(Register src1, Register src2, address addr) op3r(blt, src1, src2, addr)
AssemblyCode blt(Register src1, int immediate, address addr) op1r1i1r(blt, src1, immediate, addr)
// branch on less/equal
AssemblyCode ble(Register src1, Register src2, address addr) op3r(ble, src1, src2, addr)
AssemblyCode ble(Register src1, int immediate, address addr) op1r1i1r(ble, src1, immediate, addr)
// label
AssemblyCode setLabel(address addr) { return addr + ":"; }
// syscall
AssemblyCode syscall() { return "syscall"; }
#undef op3r
#undef op2r
#undef op2r1i
#undef op1r1i1r
/* stack frame
 * the first 4 arg is stored in a0-a4 which should be stored in stack
 * sp-before(bsp)->| tmp valueN        |
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
std::unordered_map<address, int> getStackFrame(TAClist::iterator begin, TAClist::const_iterator end) {
  std::unordered_map<address, int> ret;
  auto isTmp = [](address addr) { return addr != addrUNDEFINED && addr[0] == 't'; };
  std::set<address> tmpValue;
  std::vector<std::pair<int, address>> args;
  std::vector<address> declareInt;
  std::vector<address> declareChar;
  std::vector<std::pair<address, int>> declareIntArray;
  std::vector<std::pair<address, int>> declareCharArray;
  while (begin != end && begin->op != TACRET) {
    if (isTmp(begin->ad1)) tmpValue.emplace(begin->ad1);
    if (isTmp(begin->ad2)) tmpValue.emplace(begin->ad2);
    if (isTmp(begin->ad3)) tmpValue.emplace(begin->ad3);
    if (begin->op == TACDECLARECHARARG || begin->op == TACDECLAREINTARG)
      args.emplace_back(atoi(begin->ad1.c_str()), begin->ad3);
    if (begin->op == TACDECLAREINT)declareInt.emplace_back(begin->ad3);
    if (begin->op == TACDECLARECHAR)declareChar.emplace_back(begin->ad3);
    if (begin->op == TACDECLAREARRAYINT)declareIntArray.emplace_back(begin->ad3, atoi(begin->ad1.c_str()));
    if (begin->op == TACDECLAREARRAYCHAR)declareCharArray.emplace_back(begin->ad3, atoi(begin->ad1.c_str()));
    begin++;
  }
  int len = 0;
  // begin build arg frame
  std::sort(args.begin(), args.end());// 1 2 3 4 ...
  int cnt = args.size();
  while (args.size()) {
    auto now = args.back().second;
    args.pop_back();
    len += 4;
    ret["arg" + std::to_string(cnt)] = len;
    ret[now] = len;
    cnt--;
  }
  if (len % 4)len = (len / 4 + 1) * 4;
  for (auto&[k, v]:ret)v = len - v;
  // end build arg frame
  len = 0;
  // begin build declared value
  for (auto addr:declareChar) {
    len++;
    ret[addr] = -len;
  }
  for (auto[addr, l]:declareCharArray) {
    len += l;
    ret[addr] = -len;
  }
  if (len % 4)len = (len / 4 + 1) * 4;
  for (auto addr:declareInt) {
    len += 4;
    ret[addr] = -len;
  }
  for (auto[addr, l]:declareIntArray) {
    len += l * 4;
    ret[addr] = -len;
  }
  // end build declared value
  // begin build tmp value
  for (auto addr:tmpValue) {
    len += 4;
    ret[addr] = -len;
  }
  // end build tmp value
  len += 4;
  ret["bra"] = -len;
  len += 4;
  ret["bsp"] = -len;
  len += 4;
  ret["bfp"] = -len;
  return ret;
}
}
DefaultMIPSRegisterAllocator::DefaultMIPSRegisterAllocator(TAClist code) : RegisterAllocator(code) {
  now = this->code.begin();
}
Register DefaultMIPSRegisterAllocator::putValue2NewRegister(address value, AssemblyCodes &assemblyCodes, bool write) {
  Register reg = getNewRegister();
  if (isAddress(value)) {
    address addr;
    addr = getAddress(value);
    if (isInt(value)) {
      assemblyCodes.push_back(lw(reg, value));
      if (write)restoreRegisters[reg] = {4, value};
    } else if (isChar(value)) {
      assemblyCodes.push_back(lbu(reg, value));
      if (write)restoreRegisters[reg] = {1, value};
    } else
      unreachable();
  } else if (isString(value)) {
    unreachable();// string should be accessed by its address
  } else {
    // immediate
    assemblyCodes.push_back(li(reg, value));
    if (write)unreachable();
  }
  return reg;
}
Register DefaultMIPSRegisterAllocator::putAddress2NewRegister(address value, AssemblyCodes &assemblyCodes, bool write) {
  Register reg = getNewRegister();
  address addr;
  if (write)unreachable();
  if (isAddress(value) || isString(value)) {
    addr = getAddress(value);
  } else {
    unreachable();// address should not be accessed by its value
  }
  assemblyCodes.push_back(la(reg, addr));
  return reg;
}
void DefaultMIPSRegisterAllocator::beforeTAC(AssemblyCodes &assemblyCodes) {
  if (now->op == TACRET)buildStackFrameEnd(assemblyCodes);
}
void DefaultMIPSRegisterAllocator::afterTAC(AssemblyCodes &assemblyCodes) {
  for (auto[reg, addr]:restoreRegisters) {
    if (addr.first == 1)
      assemblyCodes.push_back(sb(reg, addr.second));
    else
      assemblyCodes.push_back(sw(reg, addr.second));
  }
  restoreRegisters.clear();
  usedRegisters.clear();
  if (now->op == TACLABEL && now->ad3.find_first_of("label") != 0) {
    stackFrame = getStackFrame(now, code.end());
    buildStackFrameBegin(assemblyCodes);
  }
  now++;
}
Register DefaultMIPSRegisterAllocator::getNewRegister() {
  Register reg;
  if (!usedRegisters.count("$t0")) {
    reg = "$t0";
  } else if (!usedRegisters.count("$t1")) {
    reg = "$t1";
  } else if (!usedRegisters.count("$t2")) {
    reg = "$t2";
  } else unreachable();
  usedRegisters.emplace(reg);
  return reg;
}
void DefaultMIPSRegisterAllocator::buildStackFrameBegin(AssemblyCodes &assemblyCodes) {
  int cnt = 0;
  for (auto[k, v]:stackFrame)if (k.find_first_of("arg") == 0)cnt++;
  // store fp
  assemblyCodes.push_back(sw(fp, getLocation(sp, -cnt * 4 + stackFrame.at("bfp"))));
  assemblyCodes.push_back(subu(fp, sp, cnt * 4));
  // store sp
  assemblyCodes.push_back(sw(sp, getLocation(fp, stackFrame.at("bsp"))));
  assemblyCodes.push_back(move(sp, fp));// in the default allocator we do not need to store registers
  // store ra
  assemblyCodes.push_back(sw(ra, getLocation(fp, stackFrame.at("bra"))));
  // store args
  for (int i = 1; i <= 4; i++) {
    auto arg = "arg" + std::to_string(i);
    if (stackFrame.count(arg))
      assemblyCodes.push_back(sw("$a" + std::to_string(i - 1), getLocation(fp, stackFrame.at(arg))));
  }
}
void DefaultMIPSRegisterAllocator::buildStackFrameEnd(AssemblyCodes &assemblyCodes) {
  // restore sp
  assemblyCodes.push_back(lw(sp, getLocation(fp, stackFrame.at("bsp"))));
  // restore ra
  assemblyCodes.push_back(lw(ra, getLocation(fp, stackFrame.at("bra"))));
  // restore fp
  assemblyCodes.push_back(lw(fp, getLocation(fp, stackFrame.at("bfp"))));
}
address DefaultMIPSRegisterAllocator::getStringName(address string) {
  if (!stringName.count(string))
    stringName[string] = "s" + std::to_string(stringName.size() + 1);
  return stringName[string];
}
Register DefaultMIPSRegisterAllocator::putValue2Location(address value,
                                                         Register physicalRegister,
                                                         AssemblyCodes &assemblyCodes) {
  if (isAddress(value)) {
    address addr = getAddress(value);
    if (isInt(value)) {
      assemblyCodes.push_back(sw(physicalRegister, addr));
    } else if (isChar(value)) {
      assemblyCodes.push_back(sb(physicalRegister, addr));
    } else unreachable();
  } else {
    unreachable();
  }
  return physicalRegister;
}
address DefaultMIPSRegisterAllocator::getAddress(address value) {
  if (isString(value))return getStringName(value);
  if (isGlobal(value))return value;
  return getLocation(fp, stackFrame[value]);
}
DefaultMIPSRegisterAllocator::DefaultMIPSRegisterAllocator() {}
