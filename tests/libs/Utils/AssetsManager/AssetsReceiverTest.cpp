#include <gtest/gtest.h>

// Other headers
#include <iostream>

namespace {

class AssetsReceiverTest : public testing::Test {
protected:
  void SetUp() override {
    std::cout << "Creating test fixture!\n";
  }

  void TearDown() override {
    std::cout << "Deleting test fixture!\n";
  }
};

TEST_F(AssetsReceiverTest, SomeDummyTestThatDoesNothing) {
  EXPECT_EQ(0, 0);
}

}