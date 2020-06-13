//
// Created by zhaoyiping on 2020/6/5.
//

#include "test.h"
#include "MIPS.h"
#include "MIPSoptimizer.h"

class TestMIPSOptimizer : public ::testing::Test {
 protected:
  void judge(AssemblyCodes &a, AssemblyCodes &b) {
    ASSERT_EQ(a.size(), b.size());
    auto aa = a.begin();
    auto bb = b.begin();
    while (aa != a.end()) {
      ASSERT_EQ(*aa, *bb);
      aa++;
      bb++;
    }
  }
};

TEST_F(TestMIPSOptimizer, loadAndStore) {
  AssemblyCodes rawCodes = {
      lw("$1", "($2)"),
      lw("$3", "($4)"),
      sw("$1", "($2)"),
      sw("$1", "($2)")
  };
  AssemblyCodes target = {
      lw("$1", "($2)"),
      lw("$3", "($4)"),
  };
  Peephole peephole;
  ASSERT_TRUE(peephole.loadAndStore(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}

TEST_F(TestMIPSOptimizer,loadAndStoreFail1){
  AssemblyCodes rawCodes = {
      lw("$1", "($2)"),
      lw("$1", "($4)"),
      sw("$1", "($2)")
  };
  AssemblyCodes target = {
      lw("$1", "($2)"),
      lw("$1", "($4)"),
      sw("$1", "($2)")
  };
  Peephole peephole;
  ASSERT_FALSE(peephole.loadAndStore(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}

TEST_F(TestMIPSOptimizer,loadAndStoreFail2){
  AssemblyCodes rawCodes = {
      lw("$1", "($2)"),
      lw("$2", "($4)"),
      sw("$1", "($2)")
  };
  AssemblyCodes target = {
      lw("$1", "($2)"),
      lw("$2", "($4)"),
      sw("$1", "($2)")
  };
  Peephole peephole;
  ASSERT_FALSE(peephole.loadAndStore(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}

TEST_F(TestMIPSOptimizer,afterJump){
  AssemblyCodes rawCodes={
      j("aaa"),
      addu("$1","$2","$3"),
      subu("$1","$2","$3"),
      setLabel("bbb"),
  };
  AssemblyCodes target={
      j("aaa"),
      setLabel("bbb"),
  };
  Peephole peephole;
  ASSERT_TRUE(peephole.afterJump(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}

TEST_F(TestMIPSOptimizer,control){
  AssemblyCodes rawCodes={
      setLabel("ddd"),
      j("aaa"),
      j("ddd"),
      setLabel("kkk"),
      j("aaa"),
  };
  AssemblyCodes target={
      setLabel("ddd"),
      j("aaa"),
      j("aaa"),
      setLabel("kkk"),
      j("aaa"),
  };
  Peephole peephole;
  ASSERT_TRUE(peephole.control(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}

TEST_F(TestMIPSOptimizer,brancheq){
  AssemblyCodes rawCodes={
      beq("$1","$2","aaa"),
      j("bbb"),
      setLabel("aaa"),
  };
  AssemblyCodes target={
      bne("$1","$2","bbb"),
      setLabel("aaa"),
  };
  Peephole peephole;
  ASSERT_TRUE(peephole.branch(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}
TEST_F(TestMIPSOptimizer,branchne){
  AssemblyCodes rawCodes={
      bne("$1","$2","aaa"),
      j("bbb"),
      setLabel("aaa"),
  };
  AssemblyCodes target={
      beq("$1","$2","bbb"),
      setLabel("aaa"),
  };
  Peephole peephole;
  ASSERT_TRUE(peephole.branch(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}
TEST_F(TestMIPSOptimizer,branchlt){
  AssemblyCodes rawCodes={
      blt("$1","$2","aaa"),
      j("bbb"),
      setLabel("aaa"),
  };
  AssemblyCodes target={
      bge("$1","$2","bbb"),
      setLabel("aaa"),
  };
  Peephole peephole;
  ASSERT_TRUE(peephole.branch(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}
TEST_F(TestMIPSOptimizer,branchgt){
  AssemblyCodes rawCodes={
      bgt("$1","$2","aaa"),
      j("bbb"),
      setLabel("aaa"),
  };
  AssemblyCodes target={
      ble("$1","$2","bbb"),
      setLabel("aaa"),
  };
  Peephole peephole;
  ASSERT_TRUE(peephole.branch(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}
TEST_F(TestMIPSOptimizer,branchge){
  AssemblyCodes rawCodes={
      bge("$1","$2","aaa"),
      j("bbb"),
      setLabel("aaa"),
  };
  AssemblyCodes target={
      blt("$1","$2","bbb"),
      setLabel("aaa"),
  };
  Peephole peephole;
  ASSERT_TRUE(peephole.branch(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}
TEST_F(TestMIPSOptimizer,branchle){
  AssemblyCodes rawCodes={
      ble("$1","$2","aaa"),
      j("bbb"),
      setLabel("aaa"),
  };
  AssemblyCodes target={
      bgt("$1","$2","bbb"),
      setLabel("aaa"),
  };
  Peephole peephole;
  ASSERT_TRUE(peephole.branch(rawCodes, rawCodes.begin()));
  judge(rawCodes, target);
}
