#include <gtest/gtest.h>

#include <TickThread/TickThread.h>

#include <iostream>
#include <chrono>

namespace {

using namespace pla::utils;

class TickThreadTestFixture : public testing::Test { };

TEST_F(TickThreadTestFixture, CheckIfTickEventOccurredAfterMinimalTimeInSeconds)
{
  auto startTime = std::chrono::high_resolution_clock::now();
  TickThread<std::chrono::seconds, 2> tickThreadSeconds;

  tickThreadSeconds.waitForTick();
  auto endTime = std::chrono::high_resolution_clock::now();

  auto timePassed = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
  EXPECT_GE(timePassed, 2);
  EXPECT_LE(timePassed, 4);
}

TEST_F(TickThreadTestFixture, CheckIfTickEventOccurredAfterMinimalTimeInMilliseconds)
{
  auto startTime = std::chrono::high_resolution_clock::now();
  TickThread<std::chrono::milliseconds , 800> tickThreadMilliseconds;

  tickThreadMilliseconds.waitForTick();
  auto endTime = std::chrono::high_resolution_clock::now();

  auto timePassed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
  EXPECT_GE(timePassed, 500);
  EXPECT_LE(timePassed, 1000);
}

int main() {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}

}
