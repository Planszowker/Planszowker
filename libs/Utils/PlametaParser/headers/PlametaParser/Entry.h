#pragma once

#include <string>
#include <utility>

namespace pla::utils::plameta {

enum class EntryType : uint8_t {
  Unknown,
  String,
  Int,
  Float
};

class Entry
{
public:
  Entry(std::string key, std::string rawValue, EntryType type);

  EntryType getType() { return m_type; }
  std::string getKey() { return m_key; }

  template<typename T>
  T getValue();

private:
  std::string m_key;
  std::string m_rawValue;
  EntryType m_type;
};

} // Namespace
