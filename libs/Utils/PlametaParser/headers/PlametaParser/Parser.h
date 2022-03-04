#pragma once

#include "Entry.h"

#include <Config.h>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>

namespace pla::utils::plameta {

class Parser
{
public:
  Parser() = delete;
  explicit Parser(std::stringstream plametaContent);

  std::shared_ptr<Entry> operator[] (const std::string& key);

private:
  void _setValidEntries();
  std::vector<std::tuple<std::string, EntryType, std::string>> m_validEntries;
  std::unordered_map<std::string, std::vector<std::shared_ptr<Entry>>> m_entries;
  std::stringstream m_plametaContent;
};

} // Namespace