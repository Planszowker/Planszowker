#include <Entry.h>

#include <sstream>

namespace pla::utils::plameta {

Entry::Entry(std::string key, std::string rawValue, EntryType type)
  : m_key(std::move(key))
  , m_rawValue(std::move(rawValue))
  , m_type(type)
{
}


template<>
int Entry::getValue<int>()
{
  int returnVal {0};

  if (m_type == EntryType::Int) {
    // Read value as int
    std::stringstream ss {m_rawValue};
    ss >> returnVal;
  }

  return returnVal;
}


template<>
std::string Entry::getValue<std::string>()
{
  if (m_type == EntryType::String) {
    return m_rawValue;
  }
  return "";
}


template<>
float Entry::getValue<float>()
{
  float returnVal {0.f};

  if (m_type == EntryType::Float) {
    // Read value as float
    std::stringstream ss {m_rawValue};
    ss >> returnVal;
  }

  return returnVal;
}

} // Namespace
