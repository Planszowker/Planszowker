#pragma once

#include <iostream>

namespace pla::common::logger {

/*!
 *  \brief Class to log additional information
 */
class Logger
{
public:

  /*!
   *  \brief Print information to sink.
   *  [TODO]: Add ability to change sinks (game console logs, etc.)
   *
   *  \param str Const reference for warning string.
   */
  static void printInfo(const std::string& str);
};

} // namespaces
