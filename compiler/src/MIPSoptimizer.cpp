//
// Created by zhaoyiping on 2020/5/16.
//

#include "MIPSoptimizer.h"

MIPSCode getMIPSCode(AssemblyCode code) {
  std::vector<std::string> vars;
  MIPSCode ret;
  int i = 0;
  for (; i < code.size(); i++)if (code[i] != ' ')ret.op += code[i]; else break;
  i++;// remove ' '
  std::string now;
  for (; i < code.size(); i++)if (code[i] == ',')vars.emplace_back(now), now = ""; else now += code[i];
  if (now != "")vars.emplace_back(now);
  if (vars.size() >= 1)ret.var1 = vars[0];
  if (vars.size() >= 2)ret.var2 = vars[1];
  if (vars.size() == 3)ret.var3 = vars[2];
  if (vars.size() > 3)unreachable();
  return ret;
}

bool MIPSisJump(MIPSCode mipsCode) {
  return mipsCode.op == "j" ||
      mipsCode.op == "jr" ||
      mipsCode.op == "jal" ||
      mipsCode.op == "beq" ||
      mipsCode.op == "bne" ||
      mipsCode.op == "bgt" ||
      mipsCode.op == "bge" ||
      mipsCode.op == "ble" ||
      mipsCode.op == "blt";
}

address getOpposite(MIPSCode mipsCode){
  if(mipsCode.op=="beq")return "bne";
  if(mipsCode.op=="bne")return "beq";
  if(mipsCode.op=="ble")return "bgt";
  if(mipsCode.op=="blt")return "bge";
  if(mipsCode.op=="bge")return "blt";
  if(mipsCode.op=="bgt")return "ble";
  unreachable();
}

std::string getJumpLabel(MIPSCode mipsCode) {
  if (!MIPSisJump(mipsCode))return "";
  if (mipsCode.op == "jr")return "";
  if (mipsCode.op == "jal" || mipsCode.op == "j")return mipsCode.var1;
  return mipsCode.var3;
}

AssemblyCode changeLabel(MIPSCode mipsCode, address label) {
  if (!MIPSisJump(mipsCode) || mipsCode.op == "jr")unreachable();
  if (mipsCode.op == "jal" || mipsCode.op == "j")
    return mipsCode.op + " " + label;
  return mipsCode.op + " " + mipsCode.var1 + "," + mipsCode.var2 + "," + label;
}

bool MIPSisLabel(MIPSCode mipsCode) {
  return mipsCode.op.back() == ':';
}

address getMIPSLabel(MIPSCode mipsCode) {
  if (!MIPSisLabel(mipsCode))unreachable();
  mipsCode.op.pop_back();
  return mipsCode.op;
}


void Peephole::optimize(AssemblyCodes &codes) {
  bool flag;
  do {
    flag = false;
    for (auto it = codes.begin(); it != codes.end(); it++) {
      flag |= loadAndStore(codes, it);
      flag |= afterJump(codes, it);
      flag |= control(codes, it);
      flag |= branch(codes, it);
    }
  } while (flag);
}
/*
 * remove store the value which is not changed
 * not erase it
 */
bool Peephole::loadAndStore(AssemblyCodes &codes, AssemblyCodes::iterator it) {
  auto head = getMIPSCode(*it);
  if (head.op != "lw" && head.op != "lbu")return false;
  if (head.var2.find(head.var1) != std::string::npos)return false;
  bool ret = false;
  for (auto now = ++it; now != codes.end(); now++) {
    auto nowMIPS = getMIPSCode(*now);
    if (MIPSisJump(nowMIPS) || MIPSisLabel(nowMIPS) || nowMIPS.op == "syscall")break;// break when end of a block
    // if sw without change then remove it
    if ((nowMIPS.op == "sw" || nowMIPS.op == "sb") && head.var1 == nowMIPS.var1 && head.var2 == nowMIPS.var2) {
      auto it = now;
      now--;
      codes.erase(it);
      ret = true;
      continue;
    }
    // if changed break
    // lw $1,($2)
    // $1 change
    if (nowMIPS.var1 == head.var1)break;
    // $2 change
    if(head.var2.find(nowMIPS.var1)!=std::string::npos)break;
  }
  return ret;
}
/*
 * jump x
 * aaa
 * bbb
 * ccc
 * label:
 * aaa bbb ccc cannot reach so remove it
 * not erase it
 */
bool Peephole::afterJump(AssemblyCodes &codes, AssemblyCodes::iterator it) {
  auto head = getMIPSCode(*it);
  bool ret = false;
  if (head.op != "j")return false;
  for (auto now = ++it; now != codes.end(); now++) {
    if (MIPSisLabel(getMIPSCode(*now)))break;
    auto tmp = now;
    now--;
    codes.erase(tmp);
    ret = true;
  }
  return ret;
}
/*
 * j labelA
 * ...
 * labelA
 * labelB
 * j labelC
 * can be changed to
 * j labelC
 * ...
 * labelA
 * labelB
 * not erase it
 */
bool Peephole::control(AssemblyCodes &codes, AssemblyCodes::iterator it) {
  auto head = getMIPSCode(*it);
  if (!MIPSisLabel(head))return false;
  head.op.pop_back();
  for (auto now = ++it; now != codes.end(); now++) {
    auto nowMIPS = getMIPSCode(*now);
    if (MIPSisLabel(nowMIPS))continue;
    if (nowMIPS.op != "j")break;
    auto label = nowMIPS.var1;
    for (auto &code:codes) {
      auto nowMIPS = getMIPSCode(code);
      if (!MIPSisJump(nowMIPS))continue;
      if (getJumpLabel(nowMIPS) != head.op)continue;
      code = changeLabel(nowMIPS, label);
    }
    return true;
  }
  return false;
}
/*
 * bne a,b,labelA
 * j labelB
 * labelC
 * labelA
 * can be changed to
 * beq a,b,labelB
 * labelC
 * labelA
 * not erase it
 */
bool Peephole::branch(AssemblyCodes &codes, AssemblyCodes::iterator it) {
  auto code1 = getMIPSCode(*it);
  if (!MIPSisJump(code1) || code1.op[0] == 'j')return false;
  auto now = it;
  now++;
  if (now == codes.end())return false;
  auto code2 = getMIPSCode(*now);
  if (code2.op != "j")return false;
  now++;
  for (; now != codes.end(); now++) {
    auto code = getMIPSCode(*now);
    if (!MIPSisLabel(code))return false;
    if (getJumpLabel(code1) == getMIPSLabel(code)) {
      *it = getOpposite(code1) + " " + code1.var1 + "," + code1.var2 + "," + getJumpLabel(code2);
      codes.erase(++it);
      return true;
    }
  }
  return false;
}
