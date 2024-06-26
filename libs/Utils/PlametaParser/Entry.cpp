#include <Entry.h>

#include <sstream>

#include <ErrorHandler/ErrorLogger.h>

namespace pla::utils::plameta {

Entry::Entry(std::string key, std::string rawValue, EntryType type)
  : m_key(std::move(key))
  , m_rawValue(std::move(rawValue))
  , m_type(type)
{
}


Entry::EntryVariant Entry::getVariant()
{
  if (m_type == EntryType::Int) {
    int returnVal {0};

    // Read value as int
    std::stringstream ss {m_rawValue};
    ss >> returnVal;

    return EntryVariant {returnVal};
  } else if (m_type == EntryType::Float) {
    float returnVal {0.f};

    std::stringstream ss {m_rawValue};
    ss >> returnVal;

    return EntryVariant {returnVal};
  } else if (m_type == EntryType::String) {
    return EntryVariant {m_rawValue};
  }

  // Throw error when type is not supported
  err_handler::ErrorLogger::throwError();
  return EntryVariant {0};
}

} // Namespace
