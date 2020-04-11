#include "base.h"
#include "iostream"
void error(const char *errorMessage) {
  std::cerr << errorMessage << std::endl;
  exit(1);
}