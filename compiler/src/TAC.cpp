//
// Created by zhaoyiping on 2020/5/2.
//

#include "TAC.h"
const address addrUNDEFINED = "";
TAC::TAC()
    : op(TACUNDEFINED), ad1(addrUNDEFINED), ad2(addrUNDEFINED), ad3(addrUNDEFINED) {

}
TAC::TAC(TACop op, address ad1, address ad2, address ad3)
    : op(op), ad1(ad1), ad2(ad2), ad3(ad3) {

}
TAC::TAC(TACop op, address ad1, address ad3)
    : op(op), ad1(ad1), ad2(addrUNDEFINED), ad3(ad3) {

}
TAC::TAC(TACop op, address ad3)
    : op(op), ad1(addrUNDEFINED), ad2(addrUNDEFINED), ad3(ad3) {

}
TAC::TAC(TACop op)
    : op(op), ad1(addrUNDEFINED), ad2(addrUNDEFINED), ad3(addrUNDEFINED) {

}
json TAC::toJSON() {
  json ret;
  ret["op"] = toString(op);
  if (ad1 != addrUNDEFINED)ret["ad1"] = ad1;
  else ret["ad1"] = "";
  if (ad2 != addrUNDEFINED)ret["ad2"] = ad2;
  else ret["ad2"] = "";
  if (ad3 != addrUNDEFINED)ret["ad3"] = ad3;
  else ret["ad3"] = "";
  return ret;
}
bool operator==(const TAC &a, const TAC &b) {
  return a.op == b.op && a.ad1 == b.ad1 && a.ad2 == b.ad2 && a.ad3 == b.ad3;
}
AddressGenerator::AddressGenerator() : tmpCnt(0) {
  allAddress.clear();
}
address AddressGenerator::newTmpAddr(Tokentype tokentype) {
  std::string addr = "t";
  if (tokentype == CHAR)addr += "c";
  else addr += "i";
  addr += std::to_string(tmpCnt + 1);
  allAddress.emplace_back(addr);
  tmpCnt++;
  return allAddress.back();
}
LabelGenerator::LabelGenerator() : tmpCnt(0) {
  allLabel.clear();
}
address LabelGenerator::newLabel() {
  allLabel.emplace_back(address("label" + std::to_string(tmpCnt + 1)));
  tmpCnt++;
  return allLabel.back();
}
void LabelGenerator::addLabel(address funcName) {
  allLabel.emplace_back(funcName);
}
TransInfo::TransInfo() :
    labelGenerator(nullptr),
    addressGenerator(nullptr),
    returnLabel(addrUNDEFINED),
    thenLabel(addrUNDEFINED),
    elseLabel(addrUNDEFINED) {}
TransInfo::TransInfo(std::shared_ptr<LabelGenerator> labelGenerator,
                     std::shared_ptr<AddressGenerator> addressGenerator,
                     address returnLabel,
                     address thenLabel,
                     address elseLabel) :
    labelGenerator(labelGenerator),
    addressGenerator(addressGenerator),
    returnLabel(returnLabel),
    thenLabel(thenLabel),
    elseLabel(elseLabel) {}

std::string toString(TACop tacOp) {
  static std::unordered_map<TACop, std::string> ma;
  if (ma.empty()) {
#define TAC2String(x) ma[x]=#x;
    TAC2String(TACADD);
    TAC2String(TACSUB);
    TAC2String(TACMUL);
    TAC2String(TACDIV);
    TAC2String(TACMOV);
    TAC2String(TACGETARR);
    TAC2String(TACSETARR);
    TAC2String(TACJLE);
    TAC2String(TACJGE);
    TAC2String(TACJNE);
    TAC2String(TACJEQ);
    TAC2String(TACJG);
    TAC2String(TACJL);
    TAC2String(TACCALL);
    TAC2String(TACRET);
    TAC2String(TACSETRET);
    TAC2String(TACGETRET);
    TAC2String(TACSETARG);
    TAC2String(TACGETARG);
    TAC2String(TACJ);
    TAC2String(TACREADINT);
    TAC2String(TACREADCHAR);
    TAC2String(TACWRITEINT);
    TAC2String(TACWRITECHAR);
    TAC2String(TACWRITESTRING);
    TAC2String(TACDECLAREINT);
    TAC2String(TACDECLARECHAR);
    TAC2String(TACDECLAREINTARG);
    TAC2String(TACDECLARECHARARG);
    TAC2String(TACDECLARESTRING);
    TAC2String(TACDECLAREARRAYINT);
    TAC2String(TACDECLAREARRAYCHAR);
    TAC2String(TACLABEL);
    TAC2String(TACUNDEFINED);
#undef TAC2String
  }
  return ma[tacOp];
}

TACop toTACop(std::string s) {
  static std::unordered_map<std::string, TACop> ma;
  if (ma.empty()) {
#define String2TAC(x) ma[#x]=x;
    String2TAC(TACADD);
    String2TAC(TACSUB);
    String2TAC(TACMUL);
    String2TAC(TACDIV);
    String2TAC(TACMOV);
    String2TAC(TACGETARR);
    String2TAC(TACSETARR);
    String2TAC(TACJLE);
    String2TAC(TACJGE);
    String2TAC(TACJNE);
    String2TAC(TACJEQ);
    String2TAC(TACJG);
    String2TAC(TACJL);
    String2TAC(TACCALL);
    String2TAC(TACRET);
    String2TAC(TACSETRET);
    String2TAC(TACGETRET);
    String2TAC(TACSETARG);
    String2TAC(TACGETARG);
    String2TAC(TACJ);
    String2TAC(TACREADINT);
    String2TAC(TACREADCHAR);
    String2TAC(TACWRITEINT);
    String2TAC(TACWRITECHAR);
    String2TAC(TACWRITESTRING);
    String2TAC(TACDECLAREINT);
    String2TAC(TACDECLARECHAR);
    String2TAC(TACDECLARESTRING);
    String2TAC(TACDECLAREARRAYINT);
    String2TAC(TACDECLAREARRAYCHAR);
    String2TAC(TACLABEL);
    String2TAC(TACUNDEFINED);
#undef String2TAC
  }
  if (ma.count(s))
    return ma[s];
  return TACUNDEFINED;
}
bool isJump(TAC tac) {
  return tac.op == TACJ ||
      tac.op == TACJL ||
      tac.op == TACJLE ||
      tac.op == TACJG ||
      tac.op == TACJGE ||
      tac.op == TACJNE ||
      tac.op == TACJEQ;
}
bool isWrite(TAC tac) {
  return tac.op == TACWRITEINT || tac.op == TACWRITESTRING || tac.op == TACWRITECHAR;
}
bool isRead(TAC tac){
  return tac.op==TACREADINT||tac.op==TACREADCHAR;
}
bool isFunctionLabel(address label) {
  return label.substr(0, 5) != "label";
}
bool isRead(address addr);
bool isDeclare(TAC tac) {
  switch (tac.op) {
    case TACDECLAREINT:
    case TACDECLARECHAR:
    case TACDECLAREINTARG:
    case TACDECLARECHARARG:
    case TACDECLAREARRAYINT:
    case TACDECLAREARRAYCHAR:
    case TACDECLARESTRING:return true;
    default:return false;
  }
}

bool isAddress(address addr) {
  return addr[0] == 'l' || addr[0] == 'g' || addr[0] == 't';
}

bool isGlobal(address addr) {
  return addr[0] == 'g';
}

bool isTmp(address addr) {
  return addr[0] == 't';
}

bool isLocal(address addr) {
  return addr[0] == 'l';
}

bool isString(address addr) {
  return addr[0] == '"' && addr.back() == '"';
}

bool isInt(address addr) {
  return addr[1] == 'i';
}

bool isChar(address addr) {
  return addr[1] == 'c';
}
