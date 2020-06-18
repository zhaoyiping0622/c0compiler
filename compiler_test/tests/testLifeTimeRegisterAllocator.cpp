//
// Created by zhaoyiping on 2020/6/14.
//

#include "test.h"
#include "LifeTimeRegisterAllocator.h"
#include "fstream"

#define TESTLIFETIMEREGISTERALLOCATORTACFILES ((std::string)RESOURCES+"generator/"+"tacJSON/")

class TestLifeTimeRegisterAllocator : public LifeTimeRegisterAllocator, public ::testing::Test {
 protected:
  TAClist readFile(std::string filename) {
    filename = TESTLIFETIMEREGISTERALLOCATORTACFILES + filename;
    std::ifstream in(filename);
    TAClist ret;
    std::string input;
    std::getline(in,input);
    json j = json::parse(input);
    for (auto t:j) {
      TAC tac;
      tac.op = toTACop(t["op"]);
      tac.ad1 = toTACop(t["ad1"]);
      tac.ad2 = toTACop(t["ad2"]);
      tac.ad3 = toTACop(t["ad3"]);
      ret.push_back(tac);
    }
    return ret;
  }
};

TEST_F(TestLifeTimeRegisterAllocator, getLifeTime) {
  TAClist input = readFile("getLifeTime.json");
}

#undef TESTLIFETIMEREGISTERALLOCATORTACFILES