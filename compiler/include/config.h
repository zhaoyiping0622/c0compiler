//
// Created by zhaoyiping on 2020/3/18.
//

#ifndef COMPILER_COMPILER_INCLUDE_CONFIG_H_
#define COMPILER_COMPILER_INCLUDE_CONFIG_H_

#include "string"
#include "fstream"
#include "argparse.hpp"
#include "base.h"
class Config {
 public:
  std::string inputFileName;
  std::string outputFileName;
  std::ifstream inputFile;
  std::ofstream outputFile;
  argparse::ArgumentParser program;
  bool debug;
  Config();
  Config(int, const char *[]);
  void output();
 private:
  void init();
  void configs();
  void setValues();
};

#endif //COMPILER_COMPILER_INCLUDE_CONFIG_H_
