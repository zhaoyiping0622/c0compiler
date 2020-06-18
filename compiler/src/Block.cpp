//
// Created by zhaoyiping on 2020/5/12.
//

#include "Block.h"
Block::Block() {}
void Block::initUEVarVarKill() {
  /* some changes:
   * setArr ad1 to VarKill ad2 ad3 to UEVar
   * add/sub/mul/div/jxx
   * mov
   * setRet/getRet/getArg/setArg/readint/readchar/writeint/writechar
   */
  auto add2VarKill = [&](address addr) {
    VarKill.insert(addr);
  };
  auto add2UEVar = [&](address addr) {
    if (VarKill.count(addr) == 0)
      UEVar.insert(addr);
  };
  for (auto code:codes) {
    switch (code.op) {
      case TACADD:
      case TACSUB:
      case TACMUL:
      case TACDIV:
      case TACGETARR:add2UEVar(code.ad1);
        add2UEVar(code.ad2);
        add2VarKill(code.ad3);
        break;
      case TACJL:
      case TACJLE:
      case TACJG:
      case TACJGE:
      case TACJEQ:
      case TACJNE:add2UEVar(code.ad1);
        add2UEVar(code.ad2);
        break;
      case TACMOV:add2UEVar(code.ad1);
        add2VarKill(code.ad3);
        break;
      case TACSETARR:add2UEVar(code.ad3);
        add2UEVar(code.ad2);
        add2VarKill(code.ad1);
        break;
      case TACSETRET:
      case TACSETARG:
      case TACWRITEINT:
      case TACWRITECHAR:add2UEVar(code.ad3);
        break;
      case TACGETRET:
      case TACGETARG:
      case TACREADINT:
      case TACREADCHAR:add2VarKill(code.ad3);
        break;
    }
  }
}
std::unordered_set<address> Block::getUpperLiveOut() {
  std::unordered_set<address> ret = UEVar;
  for (auto x:LiveOut)
    if (VarKill.count(x) == 0)
      ret.insert(x);
  return ret;
}
void Block::join(const std::unordered_set<address> &otherLiveOut) {
  for (auto addr:otherLiveOut)
    LiveOut.insert(addr);
}
// TODO: may need to create DAG entirely
void Block::DAGOptimize() {
  std::unordered_map<address, int> localLabel;
  std::unordered_map<DAGEdge, std::pair<address, int>> edge2Address;
  auto update = [&](address addr) {
    localLabel[addr] += 1;
  };
  for (auto &code:codes) {
    if (isJump(code))continue;
    if (isDeclare(code))continue;
    if (code.op == TACRET)continue;
    if (isWrite(code))continue;
    if (code.op == TACSETRET || code.op == TACSETARG || code.op == TACSETARR)continue;
    if (isRead(code)
        || code.op == TACGETARG
        || code.op == TACGETRET
        || code.op == TACGETARR
        || code.op == TACMOV) {
      update(code.ad3);
    } else if (code.op == TACCALL) {
      for (auto[k, v]:localLabel) {
        if (isGlobal(k))
          update(k);
      }
    } else {
      // add sub mul div
      DAGEdge edge;
      edge.tacOp = code.op;
      edge.ad1 = code.ad1;
      edge.ad2 = code.ad2;
      edge.label1 = localLabel[code.ad1];
      edge.label2 = localLabel[code.ad2];
      if (edge2Address.count(edge) && edge2Address[edge].second == localLabel[edge2Address[edge].first])
        code = createTAC<TACMOV>(edge2Address[edge].first, code.ad3);
      update(code.ad3);
      edge2Address[edge] = std::make_pair(code.ad3, localLabel[code.ad3]);
    }
  }
}
void Block::addLabel() {
  if (codes.front().op != TACLABEL)codes.push_front(createTAC<TACLABEL>(thisLabel));
}
void Block::removeLabel() {
  while (codes.size() && codes.front().op == TACLABEL)codes.pop_front();
}
// all TACLABEL are deleted from block
Blocks::Blocks(const TAClist &code, bool createCFG) {
  labelCnt = 0;
  split(code);
  change2NewLabels();
  for (auto block:blocks)
    block->DAGOptimize();
  if (createCFG) {
    for (auto block:blocks)
      block->initUEVarVarKill();
    initEdges();
    genAllLiveOut();
  }
}
Blocks::Blocks() {
  labelCnt = 0;
}
address Blocks::getLabel() {
  return "label" + std::to_string(++labelCnt);
}
void Blocks::storeBlock(std::shared_ptr<Block> ptr) {
  if (!ptr)return;
  for (auto label:ptr->rawLabels) {
    if (isFunctionLabel(label)) {
      if (isFunctionLabel(ptr->thisLabel))unreachable();
      ptr->thisLabel = label;
    }
  }
  for (auto label:ptr->rawLabels)rawLabel2Label[label] = ptr->thisLabel;
  label2Block[ptr->thisLabel] = ptr;
  label2Index[ptr->thisLabel] = blocks.size();
  blocks.push_back(ptr);
}
std::shared_ptr<Block> Blocks::getNewEmptyBlock() {
  auto ret = std::make_shared<Block>();
  ret->thisLabel = getLabel();
  return ret;
}
void Blocks::split(const TAClist &code) {
  std::shared_ptr<Block> ptr;
  bool readLabels = false;
  for (auto now = code.begin(); now != code.end(); now++) {
    // get raw label of this block
    if (!ptr) {
      ptr = getNewEmptyBlock();
      readLabels = true;
    }
    if (now->op == TACLABEL) {
      if (!readLabels) {
        storeBlock(ptr);
        ptr = getNewEmptyBlock();
      }
      ptr->rawLabels.emplace(now->ad3);
      readLabels = true;
      continue;
    } else {
      readLabels = false;
      ptr->codes.push_back(*now);
      if (isJump(*now) || now->op == TACRET) {
        storeBlock(ptr);
        ptr = nullptr;
      }
    }
  }
}
void Blocks::change2NewLabels() {
  for (auto &block:blocks) {
    for (auto &code:block->codes) {
      if (isJump(code))code.ad3 = rawLabel2Label[code.ad3];
    }
  }
}
void Blocks::initEdges() {
  successor.clear();
  predecessor.clear();
  successor.resize(blocks.size());
  predecessor.resize(blocks.size());
  // build CFG
  for (int i = 0; i < blocks.size(); i++) {
    auto code = *(blocks[i]->codes.rbegin());
    if (isJump(code)) {
      successor[i].push_back(label2Block[code.ad3]);
      if (code.op != TACJ && i + 1 != blocks.size())
        successor[i].push_back(blocks[i + 1]);
    } else {
      if (code.op != TACRET && i + 1 != blocks.size())
        successor[i].push_back(blocks[i + 1]);
    }
  }
  for (int i = 0; i < blocks.size(); i++) {
    for (auto block:successor[i]) {
      predecessor[label2Index[block->thisLabel]].push_back(blocks[i]);
    }
  }
}
void Blocks::genAllLiveOut() {
  bool changed = true;
  while (changed) {
    changed = false;
    for (int i = 0; i < blocks.size(); i++) {
      unsigned long size = blocks[i]->LiveOut.size();
      for (auto block:successor[i])
        blocks[i]->join(block->getUpperLiveOut());
      if (blocks[i]->LiveOut.size() > size)changed = true;
    }
  }
}
// join all tac
TAClist Blocks::getTAClist() {
  TAClist ret;
  for (auto block:blocks) {
    ret.push_back(createTAC<TACLABEL>(block->thisLabel));
    for (auto tac:block->codes) {
      ret.emplace_back(tac);
    }
  }
  return ret;
}
std::shared_ptr<Block> Blocks::operator[](int index) {
  return blocks[index];
}
