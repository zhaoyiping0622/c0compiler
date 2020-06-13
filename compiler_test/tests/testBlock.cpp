//
// Created by zhaoyiping on 2020/5/13.
//

#include "test.h"
#include "Block.h"

class BlocksTest : public Blocks, public ::testing::Test {
 protected:
  void SetUp() override {
    rawLabel2Label.clear();
    label2Block.clear();
    labelCnt = 0;
  }
};

TEST_F(BlocksTest, getLabel) {
  for (int i = 0; i < 10; i++)
    ASSERT_EQ(getLabel(), "label" + std::to_string(i + 1));
}

TEST_F(BlocksTest, split1) {
  TAClist code = {
      createTAC<TACLABEL>("main"),
      createTAC<TACDECLAREINT>("lia"),
      createTAC<TACDECLARECHAR>("lca"),
      createTAC<TACRET>()
  };
  split(code);
  ASSERT_EQ(label2Block.size(), 1);
  ASSERT_EQ(label2Block.at("main")->thisLabel, "main");
  ASSERT_EQ(label2Block.at("main")->rawLabels.size(), 1);
  ASSERT_EQ(label2Block.at("main")->rawLabels.count("main"), 1);
  code.erase(code.begin());
  ASSERT_EQ(label2Block.at("main")->codes, code);
}
TEST_F(BlocksTest, split2) {
  TAClist code = {
      createTAC<TACLABEL>("funa"),// 1
      createTAC<TACDECLAREINT>("lia"),
      createTAC<TACDECLAREINT>("lib"),
      createTAC<TACJEQ>("lia", "lib", "label1"),
      createTAC<TACJ>("label2"),// 2
      createTAC<TACLABEL>("label1"),// 3
      createTAC<TACADD>("lia", "lib", "lib"),
      createTAC<TACLABEL>("label2"),// 4
      createTAC<TACLABEL>("label3"),
      createTAC<TACRET>(),
      createTAC<TACLABEL>("main"),// 5
      createTAC<TACDECLAREINT>("lia"),
      createTAC<TACDECLARECHAR>("lca"),
      createTAC<TACCALL>("funa"),
      createTAC<TACGETRET>("lia"),
      createTAC<TACRET>()
  };
  split(code);
  ASSERT_EQ(label2Block.size(), 5);

  auto nowBlock = label2Block.at("funa");
  std::unordered_set<address> rawLabels = {"funa"};
  TAClist nowCode = {
      createTAC<TACDECLAREINT>("lia"),
      createTAC<TACDECLAREINT>("lib"),
      createTAC<TACJEQ>("lia", "lib", "label1"),
  };
  ASSERT_EQ(nowBlock->codes, nowCode);
  ASSERT_EQ(nowBlock->thisLabel, "funa");
  ASSERT_EQ(nowBlock->rawLabels, rawLabels);

  nowBlock = label2Block.at("label2");
  rawLabels = {};
  code = {
      createTAC<TACJ>("label2"),
  };
  ASSERT_EQ(nowBlock->codes, code);
  ASSERT_EQ(nowBlock->thisLabel, "label2");
  ASSERT_EQ(nowBlock->rawLabels, rawLabels);

  nowBlock = label2Block.at("label3");
  rawLabels = {"label1"};
  code = {
      createTAC<TACADD>("lia", "lib", "lib"),
  };
  ASSERT_EQ(nowBlock->codes, code);
  ASSERT_EQ(nowBlock->thisLabel, "label3");
  ASSERT_EQ(nowBlock->rawLabels, rawLabels);

  nowBlock = label2Block.at("label4");
  rawLabels = {"label2", "label3"};
  code = {
      createTAC<TACRET>(),
  };
  ASSERT_EQ(nowBlock->codes, code);
  ASSERT_EQ(nowBlock->thisLabel, "label4");
  ASSERT_EQ(nowBlock->rawLabels, rawLabels);

  nowBlock = label2Block.at("main");
  rawLabels = {"main"};
  code = {
      createTAC<TACDECLAREINT>("lia"),
      createTAC<TACDECLARECHAR>("lca"),
      createTAC<TACCALL>("funa"),
      createTAC<TACGETRET>("lia"),
      createTAC<TACRET>()
  };
  ASSERT_EQ(nowBlock->codes, code);
  ASSERT_EQ(nowBlock->thisLabel, "main");
  ASSERT_EQ(nowBlock->rawLabels, rawLabels);
}

TEST_F(BlocksTest, changeLabel) {
  TAClist code = {
      createTAC<TACLABEL>("main"),// 1
      createTAC<TACDECLAREINT>("lia"),
      createTAC<TACDECLARECHAR>("lcb"),
      createTAC<TACADD>("lia", "lcb", "lia"),
      createTAC<TACJ>("label1"),
      createTAC<TACJEQ>("lia", "lcb", "label4"),// 2
      createTAC<TACLABEL>("label1"),// 3
      createTAC<TACLABEL>("label2"),
      createTAC<TACADD>("lia", "lcb", "lia"),
      createTAC<TACJ>("label1"),
      createTAC<TACLABEL>("label3"),// 4
      createTAC<TACLABEL>("label4"),
      createTAC<TACADD>("lia", "lcb", "lia"),
      createTAC<TACRET>()
  };
  Blocks blocks(code);
  TAClist code1 = {
      createTAC<TACDECLAREINT>("lia"),
      createTAC<TACDECLARECHAR>("lcb"),
      createTAC<TACADD>("lia", "lcb", "lia"),
      createTAC<TACJ>("label3"),
  };
  TAClist code2={
      createTAC<TACJEQ>("lia", "lcb", "label4"),
  };
  TAClist code3={
      createTAC<TACADD>("lia", "lcb", "lia"),
      createTAC<TACJ>("label3"),
  };
  TAClist code4={
      createTAC<TACADD>("lia", "lcb", "lia"),
      createTAC<TACRET>()
  };
  ASSERT_EQ(blocks.blocks.size(),4);
  ASSERT_EQ(code1,blocks.blocks[0]->codes);
  ASSERT_EQ(code2,blocks.blocks[1]->codes);
  ASSERT_EQ(code3,blocks.blocks[2]->codes);
  ASSERT_EQ(code4,blocks.blocks[3]->codes);
}