#include "Entry.h"

#include <Config.h>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace pla::utils::plameta {

class Parser
{
public:
  Parser() = delete;
  explicit Parser(std::ifstream& m_plametaFileStream);

  std::shared_ptr<Entry> operator[] (const std::string& key);

private:
  void _setValidEntries();
  std::vector<std::tuple<std::string, EntryType>> m_validEntries;
  std::unordered_map<std::string, std::vector<std::shared_ptr<Entry>>> m_entries;
  std::ifstream& m_plametaFileStream;
};

} // Namespace