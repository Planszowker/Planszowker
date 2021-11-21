#include "RandomGenerator.h"

namespace pla::common::rng {

template <typename IntType>
RandomGenerator<IntType>::RandomGenerator(IntType min, IntType max)
  : m_min(min)
  , m_max(max)
  , m_randomEngine(m_randomDevice())
  , m_uniformDist(m_min, m_max)
{
}

template <typename IntType>
IntType RandomGenerator<IntType>::generateRandomNumber()
{
  return m_uniformDist(m_randomEngine);
}

template class RandomGenerator<uint8_t>;

}