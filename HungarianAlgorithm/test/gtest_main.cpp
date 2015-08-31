// gtest_main.cpp

#include <iostream>
#include "gtest/gtest.h"


using namespace std;

GTEST_API_ int main(int argc, char **argv) {
  cout <<"Running main() from gtest_main.cc\n"<<endl;
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}




