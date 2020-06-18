//
// Created by zhaoyiping on 2020/5/12.
//

#ifndef COMPILER_COMPILER_INCLUDE_BLOCK_H_
#define COMPILER_COMPILER_INCLUDE_BLOCK_H_

#include "TAC.h"
#include "unordered_set"
#include "memory"

class Block {
 public:
  /*
   * no label
   * no jump
   * has declare
   * has read write call ret
   */
  TAClist codes;
  address thisLabel;
  std::unordered_set<address> rawLabels;
  std::unordered_set<address> UEVar;
  std::unordered_set<address> VarKill;
  std::unordered_set<address> LiveOut;
  std::unordered_set<address> getUpperLiveOut();
  void join(const std::unordered_set<address> &otherLiveOut);
  Block();
  void initUEVarVarKill();
  void DAGOptimize();
  void addLabel();
  void removeLabel();
};

class DAGEdge {
 public:
  TACop tacOp;
  address ad1;
  address ad2;
  int label1;
  int label2;
};

namespace std {
template<>
class hash<DAGEdge> {
 public:
  size_t operator()(const DAGEdge &rhs) const {
    return std::hash<address>()(rhs.ad1) ^ std::hash<address>()(rhs.ad2) ^ rhs.tacOp
        ^ ((rhs.label2 << 20) | (rhs.label1 << 10));
  }
};
template <>
class equal_to<DAGEdge> {
 public:
  bool operator()(const DAGEdge &lhs, const DAGEdge &rhs) const {
    if (lhs.tacOp != rhs.tacOp)return false;
    if (lhs.ad1 == rhs.ad1 && lhs.ad2 == rhs.ad2 && lhs.label1 == rhs.label1 && lhs.label2 == rhs.label2)return true;
    if (lhs.ad2 != rhs.ad1 || lhs.ad1 != rhs.ad2 || lhs.label1 != rhs.label2 || lhs.label2 != rhs.label1)return false;
    switch (lhs.tacOp) {
      case TACADD:
      case TACMUL:return true;
      case TACSUB:
      case TACDIV:
      default:return false;
    }
  }
};
}

class Blocks {
 public:
  Blocks(const TAClist &code, bool createCFG = false);
  std::unordered_map<address, address> rawLabel2Label;
  std::unordered_map<address, std::shared_ptr<Block>> label2Block;
  std::unordered_map<address, int> label2Index;
  std::vector<std::shared_ptr<Block>> blocks;
  std::vector<std::vector<std::shared_ptr<Block>>> successor;
  std::vector<std::vector<std::shared_ptr<Block>>> predecessor;
  int labelCnt;
  void split(const TAClist &code);
  Blocks();
  // give labelx where x increase
  address getLabel();
  void storeBlock(std::shared_ptr<Block>);
  std::shared_ptr<Block> getNewEmptyBlock();
  void change2NewLabels();
  void initEdges();
  void genAllLiveOut();
  TAClist getTAClist();
  std::shared_ptr<Block> operator[](int index) ;
};

#endif //COMPILER_COMPILER_INCLUDE_BLOCK_H_
