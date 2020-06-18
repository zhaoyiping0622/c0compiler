//
// Created by zhaoyiping on 2020/5/13.
//

#include "test.h"
#include "Block.h"
#include "fstream"

#define TESTGENERATORTACJSON ((std::string)RESOURCES+"generator/"+"tacJSON/")

class BlocksTest : public Blocks, public ::testing::Test {
 protected:
  void SetUp() override {
    rawLabel2Label.clear();
    label2Block.clear();
    labelCnt = 0;
  }
  TAClist readFile(std::string filename) {
    filename = TESTGENERATORTACJSON + filename + ".json";
    std::string input;
    std::fstream in(filename);
    getline(in, input);
    json j = json::parse(input);
    TAClist ret;
    for (auto t:j) {
      std::cout << t << std::endl;
      TAC tac;
      tac.op = toTACop(t["op"]);
      tac.ad1 = t["ad1"];
      tac.ad2 = t["ad2"];
      tac.ad3 = t["ad3"];
      ret.push_back(tac);
    }
    return ret;
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
  TAClist code2 = {
      createTAC<TACJEQ>("lia", "lcb", "label4"),
  };
  TAClist code3 = {
      createTAC<TACADD>("lia", "lcb", "lia"),
      createTAC<TACJ>("label3"),
  };
  TAClist code4 = {
      createTAC<TACADD>("lia", "lcb", "lia"),
      createTAC<TACRET>()
  };
  ASSERT_EQ(blocks.blocks.size(), 4);
  ASSERT_EQ(code1, blocks.blocks[0]->codes);
  ASSERT_EQ(code2, blocks.blocks[1]->codes);
  ASSERT_EQ(code3, blocks.blocks[2]->codes);
  ASSERT_EQ(code4, blocks.blocks[3]->codes);
}

TEST_F(BlocksTest, initUEVarVarKill) {
  Block block;
  block.codes = {
      createTAC<TACADD>("lib", "lic", "lia"),
      createTAC<TACGETARR>("lid", "lie", "lib"),
      createTAC<TACJL>("lib", "lif", "label1"),
      createTAC<TACSETARR>("gca", "lib", "lid"),
      createTAC<TACSETRET>("lig"),
      createTAC<TACREADINT>("lih"),
      createTAC<TACMOV>("lii", "lij")
  };
  std::unordered_set<address> UEVar = {
      "lib", "lic", "lid", "lie", "lif", "lig", "lii"
  };
  std::unordered_set<address> VarKill = {
      "lia", "lib", "lih", "gca", "lij"
  };
  block.initUEVarVarKill();
  ASSERT_EQ(VarKill, block.VarKill);
  ASSERT_EQ(UEVar, block.UEVar);
}

TEST_F(BlocksTest, genLiveOut) {
  std::shared_ptr<Block> a = std::make_shared<Block>();
  std::shared_ptr<Block> b = std::make_shared<Block>();
  std::shared_ptr<Block> c = std::make_shared<Block>();
  std::shared_ptr<Block> d = std::make_shared<Block>();
  address lia = "lia";
  address lib = "lib";
  address lic = "lic";
  address lid = "lid";
  address lie = "lie";
  address lif = "lif";
  a->UEVar = {lia, lib, lic};
  a->VarKill = {lif};
  b->UEVar = {lia};
  b->VarKill = {lia};
  c->UEVar = {lif};
  c->VarKill = {lie};
  d->UEVar = {lid};
  d->VarKill = {lib};
  blocks = {a, b, c, d};
  successor.resize(4);
  successor[0].push_back(b);
  successor[0].push_back(c);
  successor[1].push_back(d);
  successor[2].push_back(d);
  successor[3].push_back(a);
  genAllLiveOut();
  std::unordered_set<address> aa = {lia, lic, lid, lif};
  std::unordered_set<address> bb = {lia, lic, lid};
  std::unordered_set<address> cc = {lia, lic, lid};
  std::unordered_set<address> dd = {lia, lib, lic, lid};
  ASSERT_EQ(a->LiveOut, aa);
  ASSERT_EQ(b->LiveOut, bb);
  ASSERT_EQ(c->LiveOut, cc);
  ASSERT_EQ(d->LiveOut, dd);
}

TEST_F(BlocksTest, DAGOptimize1) {
  Block b;
  b.codes = {
      createTAC<TACADD>("lia", "lib", "lid"),
      createTAC<TACDIV>("lia", "lib", "lic"),
      createTAC<TACADD>("lia", "lib", "lic"),
      createTAC<TACDIV>("lia", "lib", "lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
  };
  TAClist std = {
      createTAC<TACADD>("lia", "lib", "lid"),
      createTAC<TACDIV>("lia", "lib", "lic"),
      createTAC<TACMOV>("lid", "lic"),
      createTAC<TACDIV>("lia", "lib", "lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
  };
  b.DAGOptimize();
  ASSERT_EQ(std, b.codes);
}

TEST_F(BlocksTest, DAGOptimize2) {
  Block b;
  b.codes = {
      createTAC<TACADD>("gia", "lib", "lic"),
      createTAC<TACCALL>("funa"),
      createTAC<TACADD>("gia", "lib", "lic"),
  };
  TAClist std = {
      createTAC<TACADD>("gia", "lib", "lic"),
      createTAC<TACCALL>("funa"),
      createTAC<TACADD>("gia", "lib", "lic"),
  };
  b.DAGOptimize();
  ASSERT_EQ(b.codes, std);
}

TEST_F(BlocksTest, DAGOptimize3) {
  Block b;
  b.codes = {
      createTAC<TACSETRET>("lia"),
      createTAC<TACSETRET>("lia"),
      createTAC<TACSETRET>("1", "lia"),
  };
  TAClist std = {
      createTAC<TACSETRET>("lia"),
      createTAC<TACSETRET>("lia"),
      createTAC<TACSETRET>("1", "lia"),
  };
  b.DAGOptimize();
  ASSERT_EQ(b.codes, std);
}

TEST_F(BlocksTest, DAGOptimize4) {
  Block b;
  b.codes = {
      createTAC<TACADD>("lia", "lib", "lic"),
      createTAC<TACGETARG>("1", "lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
      createTAC<TACGETRET>("lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
      createTAC<TACGETARR>("lid", "lib", "lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
      createTAC<TACMOV>("lic", "lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
  };
  TAClist std = {
      createTAC<TACADD>("lia", "lib", "lic"),
      createTAC<TACGETARG>("1", "lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
      createTAC<TACGETRET>("lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
      createTAC<TACGETARR>("lid", "lib", "lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
      createTAC<TACMOV>("lic", "lia"),
      createTAC<TACADD>("lia", "lib", "lic"),
  };
  b.DAGOptimize();
  ASSERT_EQ(b.codes, std);
}

TEST_F(BlocksTest, total) {
  std::shared_ptr<Block> a = std::make_shared<Block>();
  std::shared_ptr<Block> b = std::make_shared<Block>();
  std::shared_ptr<Block> c = std::make_shared<Block>();
  std::shared_ptr<Block> d = std::make_shared<Block>();
  std::shared_ptr<Block> e = std::make_shared<Block>();
  std::shared_ptr<Block> f = std::make_shared<Block>();
  std::shared_ptr<Block> g = std::make_shared<Block>();
  a->codes = {
      createTAC<TACDECLAREINT>("gia"),
  };
  b->codes = {
      createTAC<TACLABEL>("funa"),
      createTAC<TACLABEL>("label1"),
      createTAC<TACRET>(),
  };
  c->codes = {
      createTAC<TACLABEL>("main"),
      createTAC<TACDECLAREINT>("lib"),
      createTAC<TACDECLAREINT>("lic"),
      createTAC<TACADD>("gia", "lib", "ti1"),
      createTAC<TACMOV>("ti1", "lic"),
      createTAC<TACCALL>("funa"),
      createTAC<TACGETRET>("ti2"),
      createTAC<TACADD>("gia", "lib", "ti3"),
      createTAC<TACMOV>("ti3", "lic"),
      createTAC<TACJL>("lib", "lic", "label3"),
  };
  d->codes = {
      createTAC<TACJ>("label4"),
  };
  e->codes = {
      createTAC<TACLABEL>("label3"),
      createTAC<TACADD>("gia", "gia", "ti4"),
      createTAC<TACMOV>("ti4", "gia"),
      createTAC<TACJ>("label5"),
  };
  f->codes = {
      createTAC<TACLABEL>("label4"),
      createTAC<TACADD>("lib", "lib", "ti5"),
      createTAC<TACMOV>("ti5", "lib"),
  };
  g->codes = {
      createTAC<TACLABEL>("label5"),
      createTAC<TACLABEL>("label2"),
      createTAC<TACRET>(),
  };
  blocks = {a, b, c, d, e, f, g};
  TAClist codes;
  for (auto block:blocks)
    for (auto code:block->codes)
      codes.push_back(code);
  Blocks anotherBlocks(codes, true);
  anotherBlocks.initEdges();
  ASSERT_EQ(blocks.size(), 7);
  std::vector<std::vector<std::shared_ptr<Block>>> std;
  std = {
      {anotherBlocks.blocks[1]},
      {},
      {anotherBlocks.blocks[4], anotherBlocks.blocks[3]},
      {anotherBlocks.blocks[5]},
      {anotherBlocks.blocks[6]},
      {anotherBlocks.blocks[6]},
      {}
  };
  ASSERT_EQ(anotherBlocks.successor, std);
  std = {
      {},
      {anotherBlocks.blocks[0]},
      {},
      {anotherBlocks.blocks[2]},
      {anotherBlocks.blocks[2]},
      {anotherBlocks.blocks[3]},
      {anotherBlocks.blocks[4], anotherBlocks.blocks[5]}
  };
  ASSERT_EQ(anotherBlocks.predecessor, std);
  TAClist target = {
      // block 1
      createTAC<TACLABEL>("label1"),
      createTAC<TACDECLAREINT>("gia"),
      // block 2
      createTAC<TACLABEL>("funa"),
      createTAC<TACRET>(),
      // block 3
      createTAC<TACLABEL>("main"),
      createTAC<TACDECLAREINT>("lib"),
      createTAC<TACDECLAREINT>("lic"),
      createTAC<TACADD>("gia", "lib", "ti1"),
      createTAC<TACMOV>("ti1", "lic"),
      createTAC<TACCALL>("funa"),
      createTAC<TACGETRET>("ti2"),
      createTAC<TACADD>("gia", "lib", "ti3"),
      createTAC<TACMOV>("ti3", "lic"),
      createTAC<TACJL>("lib", "lic", "label5"),
      // block 4
      createTAC<TACLABEL>("label4"),
      createTAC<TACJ>("label6"),
      // block 5
      createTAC<TACLABEL>("label5"),
      createTAC<TACADD>("gia", "gia", "ti4"),
      createTAC<TACMOV>("ti4", "gia"),
      createTAC<TACJ>("label7"),
      // block 6
      createTAC<TACLABEL>("label6"),
      createTAC<TACADD>("lib", "lib", "ti5"),
      createTAC<TACMOV>("ti5", "lib"),
      // block 7
      createTAC<TACLABEL>("label7"),
      createTAC<TACRET>(),
  };
  TAClist another = anotherBlocks.getTAClist();
  ASSERT_EQ(target.size(), another.size());
  ASSERT_EQ(target, another);
}