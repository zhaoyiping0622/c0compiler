//
// Created by zhaoyiping on 2020/4/8.
//

#ifndef COMPILER_COMPILER_TEST_TESTS_TEST_H_
#define COMPILER_COMPILER_TEST_TESTS_TEST_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "unordered_set"
#define RESOURCES ((std::string)"resources/")

template <typename  T>
bool operator == (const std::unordered_set<T>&a,const std::unordered_set<T>&b){
  if(a.size()!=b.size())return false;
  for(const auto &x:a)if(b.count(b)==0)return false;
  return true;
}

#endif //COMPILER_COMPILER_TEST_TESTS_TEST_H_
