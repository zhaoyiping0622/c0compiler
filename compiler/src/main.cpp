//
// Created by zhaoyiping on 2020/3/18.
//
#include "config.h"
#include "scan.h"
#include "parse.h"
#include "memory"

int main(int argc, const char *argv[]) {
  auto config = std::make_shared<Config>(argc, argv);
  auto scan = std::make_shared<Scan>(config);
  auto parse = std::make_shared<Parse>(scan);
  return 0;
}