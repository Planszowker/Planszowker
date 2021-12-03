#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

namespace pla::common::rng {

template <typename IntType>
class RandomGenerator
{
public:
  RandomGenerator(IntType min, IntType max);

  IntType generateRandomNumber();

private:
  IntType m_min;
  IntType m_max;
  std::random_device m_randomDevice;
  std::default_random_engine m_randomEngine;
  std::uniform_int_distribution<IntType> m_uniformDist;
};

}