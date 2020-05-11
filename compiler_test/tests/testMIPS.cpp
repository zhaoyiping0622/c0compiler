//
// Created by zhaoyiping on 2020/5/7.
//

#include "MIPS.h"
#include "test.h"
#include "json.hpp"
#include "fstream"
using json = nlohmann::json;
using ::testing::_;
using ::testing::Return;

#define RESOURCEGENERATOR (RESOURCES+(std::string)"generator/")
#define JSONLOCATION(x) (RESOURCEGENERATOR+(std::string)"json/"+x)

class MockRegisterAllocator : public RegisterAllocator {
 public:
  MockRegisterAllocator() {}
  MockRegisterAllocator(TAClist tacList) : RegisterAllocator(tacList) {}
  MOCK_METHOD3(putValue2NewRegister, Register(address, AssemblyCodes & , bool));
  MOCK_METHOD3(putAddress2NewRegister, Register(address, AssemblyCodes & , bool));
  MOCK_METHOD3(putValue2Location, Register(address, Register, AssemblyCodes & ));
  MOCK_METHOD1(beforeTAC, void(AssemblyCodes&));
  MOCK_METHOD1(afterTAC, void(AssemblyCodes&));
};

class MIPSTest : public ::testing::Test, public MIPSGenerator<MockRegisterAllocator> {
 protected:
  void SetUp() override {
    text.clear();
    data.clear();
    inFunction = false;
    tmp.clear();
  }
  TAClist getTAC(std::string filename) {
    filename = JSONLOCATION(filename);
    std::ifstream input(filename);
    std::string s;
    getline(input, s);
    json j = json::parse(s);
    TAClist tacList;
    for (auto x:j) {
      TAC now;
      now.op = toTACop(x["op"]);
      now.ad1 = x["ad1"];
      now.ad2 = x["ad2"];
      now.ad3 = x["ad3"];
      tacList.push_back(now);
    }
    return tacList;
  }
  MockRegisterAllocator *registerMock() {
    MockRegisterAllocator *mock_register_allocator = new MockRegisterAllocator();
    registerAllocator = std::unique_ptr<MockRegisterAllocator>(mock_register_allocator);
    return mock_register_allocator;
  }
  void TearDown() override {}
};

// a=a+a
TEST_F(MIPSTest, add122) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACADD;
  tac.ad1 = "gib";
  tac.ad2 = "gia";
  tac.ad3 = "gia";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, addu("$t0", "$t1", "$t2"));
}

// a=b+c
TEST_F(MIPSTest, add123) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACADD;
  tac.ad1 = "gia";
  tac.ad2 = "gib";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, addu("$t0", "$t1", "$t2"));
}

// c=a+1
TEST_F(MIPSTest, addimm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACADD;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, addu("$t0", "$t1", "1"));
}

// c=1+a
TEST_F(MIPSTest, addimm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACADD;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, addu("$t0", "$t1", "1"));
}

// c=c-a
TEST_F(MIPSTest, sub122) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACSUB;
  tac.ad1 = "gic";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, subu("$t0", "$t1", "$t2"));
}

// c=b-a
TEST_F(MIPSTest, sub123) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACSUB;
  tac.ad1 = "gib";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, subu("$t0", "$t1", "$t2"));
}

// c=1-a
TEST_F(MIPSTest, subimm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACSUB;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, subu("$t0", "$t1", "$t2"));
}

//c=a-1
TEST_F(MIPSTest, subimm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACSUB;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, subu("$t0", "$t1", "1"));
}

// c=c*a
TEST_F(MIPSTest, mul122) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACMUL;
  tac.ad1 = "gic";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, mul("$t0", "$t1", "$t2"));
}

// c=b*a
TEST_F(MIPSTest, mul123) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACMUL;
  tac.ad1 = "gib";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, mul("$t0", "$t1", "$t2"));
}

// c=1*a
TEST_F(MIPSTest, mulimm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACMUL;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, mul("$t0", "$t1", "1"));
}

//c=a*1
TEST_F(MIPSTest, mulimm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACMUL;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, mul("$t0", "$t1", "1"));
}

// c=c/a
TEST_F(MIPSTest, div122) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACDIV;
  tac.ad1 = "gic";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, div("$t0", "$t1", "$t2"));
}

// c=b/a
TEST_F(MIPSTest, div123) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACDIV;
  tac.ad1 = "gib";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, div("$t0", "$t1", "$t2"));
}

// c=1/a
TEST_F(MIPSTest, divimm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACDIV;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"))
      .WillOnce(Return("$t2"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, div("$t0", "$t1", "$t2"));
}

// c=a/1
TEST_F(MIPSTest, divimm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACDIV;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, div("$t0", "$t1", "1"));
}

TEST_F(MIPSTest, mov12) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACMOV;
  tac.ad1 = "gia";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, move("$t0", "$t1"));
}

TEST_F(MIPSTest, movimm) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACMOV;
  tac.ad1 = "1";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  ASSERT_EQ(acs.size(), 1);
  auto ac = *acs.begin();
  ASSERT_EQ(ac, li("$t0", "1"));
}

TEST_F(MIPSTest, getArrInt123) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACGETARR;
  tac.ad1 = "gia";
  tac.ad2 = "gib";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t2"));
  EXPECT_CALL((*mock_register_allocator), putAddress2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      sll("$t2", "$t2", 2),
      addu("$t1", "$t1", "$t2"),
      lw("$t0", getLocation("$t1", 0))
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, getArrChar123) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACGETARR;
  tac.ad1 = "gca";
  tac.ad2 = "gib";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t2"));
  EXPECT_CALL((*mock_register_allocator), putAddress2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      addu("$t1", "$t1", "$t2"),
      lw("$t0", getLocation("$t1", 0))
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, getArrCharImm) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACGETARR;
  tac.ad1 = "gca";
  tac.ad2 = "2";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putAddress2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      lw("$t0", getLocation("$t1", 2))
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, getArrIntImm) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACGETARR;
  tac.ad1 = "gia";
  tac.ad2 = "2";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putAddress2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      lw("$t0", getLocation("$t1", 8))
  };
  ASSERT_EQ(acs, std);
}
// a[b]=c
TEST_F(MIPSTest, setArrChar123) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACSETARR;
  tac.ad1 = "gca";
  tac.ad2 = "gib";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t2"));
  EXPECT_CALL((*mock_register_allocator), putAddress2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      addu("$t1", "$t1", "$t2"),
      sw("$t0", getLocation("$t1", 0))
  };
  ASSERT_EQ(acs, std);
}
// a[b]=c
TEST_F(MIPSTest, setArrInt123) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACSETARR;
  tac.ad1 = "gia";
  tac.ad2 = "gib";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t2"));
  EXPECT_CALL((*mock_register_allocator), putAddress2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      sll("$t2", "$t2", 2),
      addu("$t1", "$t1", "$t2"),
      sw("$t0", getLocation("$t1", 0))
  };
  ASSERT_EQ(acs, std);
}
// a[2]=c
TEST_F(MIPSTest, setArrCharImm) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACSETARR;
  tac.ad1 = "gca";
  tac.ad2 = "2";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putAddress2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      sw("$t0", getLocation("$t1", 2))
  };
  ASSERT_EQ(acs, std);
}
// a[2]=c
TEST_F(MIPSTest, setArrIntImm) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACSETARR;
  tac.ad1 = "gia";
  tac.ad2 = "2";
  tac.ad3 = "gic";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  EXPECT_CALL((*mock_register_allocator), putAddress2NewRegister(_, _, false))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      sw("$t0", getLocation("$t1", 8))
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, Jle12) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJLE;
  tac.ad1 = "gia";
  tac.ad2 = "gib";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      ble("$t0", "$t1", "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JleImm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJLE;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      ble("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JleImm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJLE;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      bge("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, Jge12) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJGE;
  tac.ad1 = "gia";
  tac.ad2 = "gib";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      bge("$t0", "$t1", "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JgeImm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJGE;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      bge("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JgeImm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJGE;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      ble("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, Jl12) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJL;
  tac.ad1 = "gia";
  tac.ad2 = "gib";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      blt("$t0", "$t1", "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JlImm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJL;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      blt("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JlImm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJL;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      bgt("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, Jg12) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJG;
  tac.ad1 = "gia";
  tac.ad2 = "gib";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      bgt("$t0", "$t1", "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JgImm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJG;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      bgt("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JgImm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJG;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      blt("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, Jne12) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJNE;
  tac.ad1 = "gia";
  tac.ad2 = "gib";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      bne("$t0", "$t1", "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JneImm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJNE;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      bne("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JneImm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJNE;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      bne("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, Jeq12) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJEQ;
  tac.ad1 = "gia";
  tac.ad2 = "gib";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t1"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      beq("$t0", "$t1", "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JeqImm1) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJEQ;
  tac.ad1 = "gia";
  tac.ad2 = "1";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      beq("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}
TEST_F(MIPSTest, JeqImm2) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACJEQ;
  tac.ad1 = "1";
  tac.ad2 = "gia";
  tac.ad3 = "label1";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      beq("$t0", 1, "label1")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, Call) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACCALL;
  tac.ad3 = "main";
  argLen = 100;
  auto acs = translate(tac);
  AssemblyCodes std = {
      jal("main")
  };
  ASSERT_EQ(acs, std);
  ASSERT_EQ(argLen, 0);
}

TEST_F(MIPSTest, Ret) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACRET;
  auto acs = translate(tac);
  AssemblyCodes std = {
      jr(ra)
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, SetRetValue) {
  auto mock_register_allocator = registerMock();
  EXPECT_CALL(*mock_register_allocator, putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  TAC tac;
  tac.op = TACSETRET;
  tac.ad3 = "gia";
  auto acs = translate(tac);
  AssemblyCodes std = {
      move("$v0", "$t0")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, SetRetImm) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACSETRET;
  tac.ad3 = "1";
  auto acs = translate(tac);
  AssemblyCodes std = {
      li("$v0", "1")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, GetRetValue) {
  auto mock_register_allocator = registerMock();
  EXPECT_CALL(*mock_register_allocator, putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"));
  TAC tac;
  tac.op = TACGETRET;
  tac.ad3 = "gia";
  auto acs = translate(tac);
  AssemblyCodes std = {
      move("$t0", "$v0")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, SetArg1_4) {
  auto mock_register_allocator = registerMock();
  EXPECT_CALL(*mock_register_allocator, putValue2NewRegister(_, _, false))
      .Times(2)
      .WillRepeatedly(Return("$t0"));
  TAC tac;
  tac.op = TACSETARG;
  tac.ad3 = "gia";
  tac.ad1 = "1";
  auto acs = translate(tac);
  AssemblyCodes std = {
      move("$a0", "$t0")
  };
  ASSERT_EQ(acs, std);
  tmp.clear();
  tac.ad1 = "2";
  acs = translate(tac);
  std = {
      move("$a1", "$t0")
  };
  ASSERT_EQ(acs, std);
  tmp.clear();
  tac.ad3 = "1";
  tac.ad1 = "3";
  acs = translate(tac);
  std = {
      li("$a2", "1")
  };
  ASSERT_EQ(acs, std);
  tmp.clear();
  tac.ad1 = "4";
  acs = translate(tac);
  std = {
      li("$a3", "1")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, SetArgGe5) {
  auto mock_register_allocator = registerMock();
  EXPECT_CALL(*mock_register_allocator, putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t0"));
  TAC tac;
  tac.op = TACSETARG;
  tac.ad3 = "gia";
  tac.ad1 = "10";
  auto acs = translate(tac);
  tac.ad1 = "9";
  acs = translate(tac);
  AssemblyCodes std = {
      sw("$t0", getLocation(sp, -4)),
      sw("$t0", getLocation(sp, -8)),
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, GetArg) {
  auto mock_register_allocator = registerMock();
  EXPECT_CALL(*mock_register_allocator, putValue2NewRegister(_, _, true))
      .WillOnce(Return("$t0"))
      .WillOnce(Return("$t0"));
  TAC tac;
  tac.op = TACGETARG;
  tac.ad3 = "gia";
  tac.ad1 = "1";
  auto acs = translate(tac);
  tac.ad1 = "2";
  acs = translate(tac);
  AssemblyCodes std = {
      lw("$t0", getLocation(fp, 0)),
      lw("$t0", getLocation(fp, 4))
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, J) {
  TAC tac;
  tac.op = TACJ;
  tac.ad3 = "main";
  auto acs = translate(tac);
  AssemblyCodes std = {
      j("main")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, ReadInt) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACREADINT;
  tac.ad3 = "gia";
  EXPECT_CALL((*mock_register_allocator), putValue2Location("gia", "$v0", _))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      li("$v0", "5"),
      syscall(),
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, ReadChar) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACREADCHAR;
  tac.ad3 = "gca";
  EXPECT_CALL((*mock_register_allocator), putValue2Location("gca", "$v0", _))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      li("$v0", "12"),
      syscall(),
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, WriteIntValue) {
  auto mock_register_allocator = registerMock();
  TAC tac;
  tac.op = TACWRITEINT;
  tac.ad3 = "gia";
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  auto acs = translate(tac);
  AssemblyCodes std = {
      move("$a0", "$t0"),
      li("$v0", "1"),
      syscall(),
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, WriteIntImm) {
  TAC tac;
  tac.op = TACWRITEINT;
  tac.ad3 = "2";
  auto acs = translate(tac);
  AssemblyCodes std = {
      li("$a0", "2"),
      li("$v0", "1"),
      syscall(),
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, WriteCharValue) {
  auto mock_register_allocator = registerMock();
  EXPECT_CALL((*mock_register_allocator), putValue2NewRegister(_, _, false))
      .WillOnce(Return("$t0"));
  TAC tac;
  tac.op = TACWRITECHAR;
  tac.ad3 = "gca";
  auto acs = translate(tac);
  AssemblyCodes std = {
      move("$a0", "$t0"),
      li("$v0", "11"),
      syscall(),
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, WriteCharImm) {
  TAC tac;
  tac.op = TACWRITECHAR;
  tac.ad3 = "2";
  auto acs = translate(tac);
  AssemblyCodes std = {
      li("$a0", "2"),
      li("$v0", "11"),
      syscall(),
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, WriteString) {
  auto mock_register_allocator = registerMock();
  EXPECT_CALL((*mock_register_allocator), putAddress2NewRegister("\"helloworld\"", _, false))
      .WillOnce(Return("$t0"));
  TAC tac;
  tac.op = TACWRITESTRING;
  tac.ad3 = "\"helloworld\"";
  auto acs = translate(tac);
  AssemblyCodes std = {
      move("$a0", "$t0"),
      li("$v0", "4"),
      syscall()
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, Label) {
  TAC tac;
  tac.op = TACLABEL;
  tac.ad3 = "main";
  auto acs = translate(tac);
  AssemblyCodes std = {
      setLabel("main")
  };
  ASSERT_EQ(acs, std);
}

TEST_F(MIPSTest, DeclareInt) {
  TAC tac;
  tac.op = TACDECLAREINT;
  tac.ad3 = "gia";
  translate(tac);
  AssemblyCodes std = {
      tac.ad3 + ": .word 0"
  };
  ASSERT_EQ(data, std);
}

TEST_F(MIPSTest, DeclareChar) {
  TAC tac;
  tac.op = TACDECLARECHAR;
  tac.ad3 = "gca";
  translate(tac);
  AssemblyCodes std = {
      tac.ad3 + ": .byte 0"
  };
  ASSERT_EQ(data, std);
}

TEST_F(MIPSTest, DeclareArrayInt) {
  TAC tac;
  tac.op = TACDECLAREARRAYINT;
  tac.ad3 = "gia";
  tac.ad1 = "10";
  translate(tac);
  AssemblyCodes std = {
      tac.ad3 + ": .space 40"
  };
  ASSERT_EQ(data, std);
}

TEST_F(MIPSTest, DeclareArrayChar) {
  TAC tac;
  tac.op = TACDECLAREARRAYCHAR;
  tac.ad3 = "gca";
  tac.ad1 = "10";
  translate(tac);
  AssemblyCodes std = {
      tac.ad3 + ": .space 10"
  };
  ASSERT_EQ(data, std);
}

TEST_F(MIPSTest, DeclareString) {
  TAC tac;
  tac.op = TACDECLARESTRING;
  tac.ad3 = "\"helloworld\"";
  translate(tac);
  tac.ad3 = "\"helloworld!\"";
  translate(tac);
  AssemblyCodes std = {
      "s1: .ascii \"helloworld\\0\"",
      "s2: .ascii \"helloworld!\\0\"",
  };
  ASSERT_EQ(data, std);
}

TEST_F(MIPSTest, StackFrameNoArg) {
  TAClist tacList = {
      createTAC<TACLABEL>("main"),
      createTAC<TACDECLAREINT>("lia"),
      createTAC<TACDECLAREINT>("lib"),
      createTAC<TACDECLARECHAR>("lcaa"),
      createTAC<TACDECLARECHAR>("lcab"),
      createTAC<TACDECLARECHAR>("lcac"),
      createTAC<TACDECLAREARRAYINT>("10", "lix"),
      createTAC<TACDECLAREARRAYINT>("1", "lixx"),
      createTAC<TACDECLAREARRAYCHAR>("10", "lcy"),
      createTAC<TACDECLAREARRAYCHAR>("1", "lcyy"),
      createTAC<TACADD>("lia", "lib", "ti1"),
      createTAC<TACADD>("lia", "lib", "ti2"),
      createTAC<TACADD>("lia", "lib", "ti3"),
      createTAC<TACRET>(),
  };
  auto stackFrame = getStackFrame(tacList.begin(), tacList.end());
  std::unordered_map<address, int> std = {
      {"lcaa", -1},
      {"lcab", -2},
      {"lcac", -3},
      {"lcy", -13},
      {"lcyy", -14},
      {"lia", -20},
      {"lib", -24},
      {"lix", -64},
      {"lixx", -68},
      {"ti1", -72},
      {"ti2", -76},
      {"ti3", -80},
      {"bra", -84},
      {"bsp", -88},
      {"bfp", -92},
  };
  ASSERT_EQ(std.size(), stackFrame.size());
  for (auto[k, v]:std) {
    ASSERT_EQ(stackFrame.count(k), 1);
    ASSERT_EQ(v, stackFrame[k]) << k << " " << v << " " << stackFrame[k] << std::endl;
  }
}

TEST_F(MIPSTest, StackFrame2Args) {
  TAClist tacList = {
      createTAC<TACLABEL>("main"),
      createTAC<TACDECLAREINTARG>("1", "liarga"),
      createTAC<TACDECLARECHARARG>("2", "lcargb"),
      createTAC<TACDECLAREINT>("lia"),
      createTAC<TACDECLAREINT>("lib"),
      createTAC<TACDECLARECHAR>("lcaa"),
      createTAC<TACDECLARECHAR>("lcab"),
      createTAC<TACDECLARECHAR>("lcac"),
      createTAC<TACDECLAREARRAYINT>("10", "lix"),
      createTAC<TACDECLAREARRAYINT>("1", "lixx"),
      createTAC<TACDECLAREARRAYCHAR>("10", "lcy"),
      createTAC<TACDECLAREARRAYCHAR>("1", "lcyy"),
      createTAC<TACADD>("lia", "lib", "ti1"),
      createTAC<TACADD>("lia", "lib", "ti2"),
      createTAC<TACADD>("lia", "lib", "ti3"),
      createTAC<TACRET>(),
  };
  auto stackFrame = getStackFrame(tacList.begin(), tacList.end());
  std::unordered_map<address, int> std = {
      {"liarga", 0},// 0 1 2 3
      {"lcargb", 4},// 4 5 6 7
      {"arg1", 0},
      {"arg2", 4},
      {"lcaa", -1},
      {"lcab", -2},
      {"lcac", -3},
      {"lcy", -13},
      {"lcyy", -14},
      {"lia", -20},
      {"lib", -24},
      {"lix", -64},
      {"lixx", -68},
      {"ti1", -72},
      {"ti2", -76},
      {"ti3", -80},
      {"bra", -84},
      {"bsp", -88},
      {"bfp", -92},
  };
  ASSERT_EQ(std.size(), stackFrame.size());
  for (auto[k, v]:std) {
    ASSERT_EQ(stackFrame.count(k), 1) << k << std::endl;
    ASSERT_EQ(v, stackFrame[k]) << k << " " << v << " " << stackFrame[k] << std::endl;
  }
}

class DefaultMIPSRegisterAllocatorTest : public DefaultMIPSRegisterAllocator, public ::testing::Test {
 protected:
  void SetUp() override {
    data.clear();
    stringName.clear();
    restoreRegisters.clear();
    usedRegisters.clear();
    stackFrame.clear();
  }
  void TearDown() override {

  }
};

TEST_F(DefaultMIPSRegisterAllocatorTest, getAddress) {
  ASSERT_EQ(getAddress("\"helloworld\""), "s1");
  ASSERT_EQ(getAddress("\"helloworld\""), "s1");
  ASSERT_EQ(getAddress("\"helloworld!\""), "s2");
  ASSERT_EQ(getAddress("gia"), "gia");
  ASSERT_EQ(getAddress("gca"), "gca");
  std::unordered_map<address, int> std = {
      {"lia", 4},
      {"lcb", 5}
  };
  stackFrame = std;
  ASSERT_EQ(getAddress("lia"), getLocation(fp, 4));
  ASSERT_EQ(getAddress("lcb"), getLocation(fp, 5));
}

TEST_F(DefaultMIPSRegisterAllocatorTest, getNewRegister) {
  ASSERT_EQ(getNewRegister(), "$t0");
  ASSERT_EQ(getNewRegister(), "$t1");
  ASSERT_EQ(getNewRegister(), "$t2");
  ASSERT_THROW(getNewRegister(), BaseError);
}

TEST_F(DefaultMIPSRegisterAllocatorTest, putValue2NewRegister1) {
  AssemblyCodes code;
  ASSERT_EQ(putValue2NewRegister("gia", code, true), "$t0");
  ASSERT_EQ(putValue2NewRegister("gca", code, true), "$t1");
  ASSERT_EQ(putValue2NewRegister("gca", code, false), "$t2");
  ASSERT_EQ(restoreRegisters["$t0"], std::make_pair(4, (std::string)"gia"));
  ASSERT_EQ(restoreRegisters["$t1"], std::make_pair(1, (std::string)"gca"));
  ASSERT_EQ(restoreRegisters.size(), 2);
  AssemblyCodes std = {
      lw("$t0", "gia"),
      lbu("$t1", "gca"),
      lbu("$t2", "gca"),
  };
  ASSERT_EQ(code, std);
}
TEST_F(DefaultMIPSRegisterAllocatorTest, putValue2NewRegister2) {
  AssemblyCodes code;
  ASSERT_EQ(putValue2NewRegister("gia", code, false), "$t0");
  ASSERT_EQ(putValue2NewRegister("12", code, false), "$t1");
  ASSERT_EQ(restoreRegisters.size(), 0);
  AssemblyCodes std = {
      lw("$t0", "gia"),
      li("$t1", "12")
  };
  ASSERT_EQ(code, std);
  ASSERT_THROW(putValue2NewRegister("\"helloworld\"", code, false), BaseError);
  usedRegisters.clear();
  ASSERT_THROW(putValue2NewRegister("12", code, true), BaseError);
}
TEST_F(DefaultMIPSRegisterAllocatorTest, putAddress2NewRegister) {
  AssemblyCodes code;
  ASSERT_EQ(putAddress2NewRegister("\"helloworld\"", code, false), "$t0");
  ASSERT_EQ(putAddress2NewRegister("gia", code, false), "$t1");
  AssemblyCodes std = {
      la("$t0", "s1"),
      la("$t1", "gia"),
  };
  ASSERT_EQ(std, code);
  usedRegisters.clear();
  ASSERT_THROW(putAddress2NewRegister("gia", code, true), BaseError);
  ASSERT_THROW(putAddress2NewRegister("12", code, false), BaseError);
}
TEST_F(DefaultMIPSRegisterAllocatorTest, putValue2Location) {
  AssemblyCodes code;
  ASSERT_EQ(putValue2Location("gia", "$t0", code), "$t0");
  ASSERT_EQ(putValue2Location("gca", "$t1", code), "$t1");
  AssemblyCodes std = {
      sw("$t0", "gia"),
      sb("$t1", "gca"),
  };
  ASSERT_EQ(code, std);
  ASSERT_THROW(putValue2Location("\"helloworld\"", "$t0", code), BaseError);
  ASSERT_THROW(putValue2Location("1", "$t0", code), BaseError);
}
TEST_F(DefaultMIPSRegisterAllocatorTest, buildStackFrame) {
  AssemblyCodes code;
  TAClist tacList = {
      createTAC<TACLABEL>("fun1"),
      createTAC<TACDECLAREINTARG>("liarga"),
      createTAC<TACDECLAREINTARG>("liargb"),
      createTAC<TACDECLAREINTARG>("liargc"),
      createTAC<TACDECLAREINTARG>("liargd"),
      createTAC<TACDECLAREINTARG>("liarge"),
      createTAC<TACDECLAREINT>("lix"),
      createTAC<TACRET>()
  };
  auto stackFrame = getStackFrame(tacList.begin(), tacList.end());
  this->code = tacList;
  now = this->code.begin();
  afterTAC(code);
  now = --tacList.end();
  beforeTAC(code);
  AssemblyCodes std = {
      sw(fp, getLocation(sp, -20 + stackFrame.at("bfp"))),
      subu(fp, sp, 20),
      sw(sp, getLocation(fp, stackFrame.at("bsp"))),
      move(sp, fp),
      sw(ra, getLocation(fp, stackFrame.at("bra"))),
      sw("$a0", getLocation(fp, stackFrame.at("arg1"))),
      sw("$a1", getLocation(fp, stackFrame.at("arg2"))),
      sw("$a2", getLocation(fp, stackFrame.at("arg3"))),
      sw("$a3", getLocation(fp, stackFrame.at("arg4"))),
      lw(sp, getLocation(fp, stackFrame.at("bsp"))),
      lw(ra, getLocation(fp, stackFrame.at("bra"))),
      lw(fp, getLocation(fp, stackFrame.at("bfp"))),
  };
  ASSERT_EQ(code, std);
}
TEST_F(DefaultMIPSRegisterAllocatorTest, afterTAC) {
  TAClist tacList = {
      createTAC<TACADD>("tia", "tib", "tcc")
  };
  now = tacList.begin();
  AssemblyCodes code;
  ASSERT_EQ(putValue2NewRegister("tia", code, true), "$t0");
  ASSERT_EQ(putValue2NewRegister("tib", code, true), "$t1");
  ASSERT_EQ(putValue2NewRegister("tcc", code, true), "$t2");
  code.clear();
  afterTAC(code);
  AssemblyCodes std = {
      sw("$t0", "tia"),
      sw("$t1", "tib"),
      sb("$t2", "tcc"),
  };
  std.sort();
  code.sort();
  ASSERT_EQ(std, code);
}
