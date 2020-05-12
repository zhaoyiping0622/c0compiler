//
// Created by zhaoyiping on 2020/3/18.
//
#include "config.h"
#include "scan.h"
#include "parse.h"
#include "MIPS.h"
#include "memory"

int main(int argc, const char *argv[]) {
  auto config = std::make_shared<Config>(argc, argv);
  auto scan = std::make_shared<Scan>(config);
  auto parse = std::make_shared<Parse>(scan);
  std::shared_ptr<AST> ASTRoot;
  try {
    ASTRoot = parse->run();
  } catch (std::exception &e) {
    std::cerr << "at line" << scan->lineNumber << std::endl;
    std::cerr << e.what() << std::endl;
    return 1;
  }
  if (config->ast) {
    config->outputFile << ASTRoot->toJSON(true) << std::endl;
    return 0;
  }
  TAClist ThreeAddressCodes;
  TransInfo transInfo(std::make_shared<LabelGenerator>(), std::make_shared<AddressGenerator>());
  for (auto now = ASTRoot; now; now = now->next)now->toTAC(ThreeAddressCodes, transInfo);
  if (config->tac) {
    json j;
    for (auto x:ThreeAddressCodes) {
      j.push_back(x.toJSON());
    }
    config->outputFile << j << std::endl;
    return 0;
  }
  MIPSGenerator<DefaultMIPSRegisterAllocator> mipsGenerator;
  AssemblyCodes assemblyCodes = mipsGenerator.translate(ThreeAddressCodes);
  for (auto line:assemblyCodes)config->outputFile << line << std::endl;
  return 0;
}