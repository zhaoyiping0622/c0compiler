//
// Created by zhaoyiping on 2020/5/2.
//

#ifndef COMPILER_COMPILER_INCLUDE_TAC_H_
#define COMPILER_COMPILER_INCLUDE_TAC_H_

#include "base.h"
#include "memory"
#include "list"
#include "vector"
#include "token.h"
#include "json.hpp"
extern const address addrUNDEFINED;
using json = nlohmann::json;

enum emTACOP : int {
  TACADD,
  TACSUB,
  TACMUL,
  TACDIV,
  TACMOV,
  TACGETARR,
  TACSETARR,
  TACJLE,
  TACJGE,
  TACJNE,
  TACJEQ,
  TACJG,
  TACJL,
  TACCALL,
  TACRET,
  TACSETRET,
  TACGETRET,
  TACSETARG,
  TACGETARG,
  TACJ,
  TACREADINT,
  TACREADCHAR,
  TACWRITEINT,
  TACWRITECHAR,
  TACWRITESTRING,
  TACDECLAREINT,
  TACDECLARECHAR,
  TACDECLAREINTARG,
  TACDECLARECHARARG,
  TACDECLARESTRING,
  TACDECLAREARRAYINT,
  TACDECLAREARRAYCHAR,
  TACLABEL,
  TACUNDEFINED
};

std::string toString(TACop);
TACop toTACop(std::string);

class TAC {
 public:
  TACop op;
  address ad1;
  address ad2;
  address ad3;
  TAC();
  TAC(TACop op, address ad1, address ad2, address ad3);
  TAC(TACop op, address ad1, address ad3);
  TAC(TACop op, address ad3);
  TAC(TACop op);
  json toJSON();
};

template<TACop tacOp, typename...Types>
TAC createTAC(Types ...args) {
  return TAC(tacOp, args...);
}

// temporary variable start with `t`
// global variable start with `g`
// local variable start with `l`
class AddressGenerator {
 private:
  std::vector<address> allAddress;
  int tmpCnt;
 public:
  AddressGenerator();
  address newTmpAddr(Tokentype tokentype = INT);
};

class LabelGenerator {
 private:
  std::vector<address> allLabel;
  int tmpCnt;
 public:
  LabelGenerator();
  address newLabel();
  void addLabel(address funcName);
};

class TransInfo {
 public:
  std::shared_ptr<LabelGenerator> labelGenerator;
  std::shared_ptr<AddressGenerator> addressGenerator;
  address returnLabel;
  address thenLabel;
  address elseLabel;
  TransInfo();
  TransInfo(
      std::shared_ptr<LabelGenerator> labelGenerator,
      std::shared_ptr<AddressGenerator> addressGenerator,
      address returnLabel = addrUNDEFINED,
      address thenLabel = addrUNDEFINED,
      address elseLabel = addrUNDEFINED);
};

#endif // COMPILER_COMPILER_INCLUDE_TAC_H_