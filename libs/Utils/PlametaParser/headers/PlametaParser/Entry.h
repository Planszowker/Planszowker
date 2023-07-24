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

  [[nodiscard]] [[maybe_unused]]
  EntryType getType() { return m_type; }

  [[nodiscard]] [[maybe_unused]]
  std::string getKey() { return m_key; }

  [[nodiscard]] [[maybe_unused]]
  std::string getRaw() { return m_rawValue; }

  [[nodiscard]]
  EntryVariant getVariant();

private:
  std::string m_key;
  std::string m_rawValue;
  EntryType m_type;
};

} // Namespace
