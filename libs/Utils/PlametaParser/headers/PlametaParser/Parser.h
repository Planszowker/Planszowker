#pragma once

#include "Entry.h"

#include <Config.h>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <sstream>

namespace pla::utils::plameta {

/*!
 * @brief PlaMeta Parser class
 * This class is responsible for parsing `.plameta` file
 *
 * @ingroup plameta
 */
class Parser
{
public:

  Parser() = delete;

  /*!
   * @brief Parser class constructor
   *
   * @param[in] plametaContent StringStream of `.plameta` file.
   * Content will be read from this stream.
   */
  explicit Parser(std::stringstream plametaContent);


  /*!
   * @brief Copy constructor
   *
   * @param[in] other Other parser to be copied from.
   */
  Parser(const Parser& other);

  /*!
   * @brief Operator [] overloading
   * This allows for accessing `.plameta` file conveniently with
   * syntax of `Parser["global_key:key_name"]
   *
   * @ingroup plameta
   */
  std::shared_ptr<Entry> operator[] (const std::string& key) const;

  friend std::ostream& operator<<(std::ostream& os, Parser const& parser) {
    for (auto [entryName, entryVec] : parser.m_entries) {
      os << entryName << "\n";
    }
    return os;
  }

private:
  void _setValidEntries();
  std::vector<std::tuple<std::string, EntryType, std::string>> m_validEntries;
  std::unordered_map<std::string, std::vector<std::shared_ptr<Entry>>> m_entries;
  std::stringstream m_plametaContent;
};

} // Namespace