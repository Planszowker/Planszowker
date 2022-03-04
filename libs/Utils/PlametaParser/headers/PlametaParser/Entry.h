#pragma once

#include <string>
#include <utility>
#include <variant>

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
  using EntryVariant = std::variant<int, float, std::string>;
  Entry(std::string key, std::string rawValue, EntryType type);

  EntryType getType() { return m_type; }
  std::string getKey() { return m_key; }
  std::string getRaw() { return m_rawValue; }

  EntryVariant getVariant();

private:
  std::string m_key;
  std::string m_rawValue;
  EntryType m_type;
};

} // Namespace
