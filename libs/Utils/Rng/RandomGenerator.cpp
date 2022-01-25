#include "RandomGenerator.h"

namespace pla::common::rng {

RandomGenerator::RandomGenerator(int min, int max)
  : m_min(min)
  , m_max(max)
  , m_randomEngine(m_randomDevice())
  , m_uniformDist(m_min, m_max)
{
}

int RandomGenerator::generateRandomNumber()
{
  return m_uniformDist(m_randomEngine);
}

}