#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

namespace pla::common::rng {

class RandomGenerator
{
public:
  RandomGenerator(int min, int max);

  int generateRandomNumber();

private:
  int m_min;
  int m_max;
  std::random_device m_randomDevice;
  std::default_random_engine m_randomEngine;
  std::uniform_int_distribution<int> m_uniformDist;
};

}