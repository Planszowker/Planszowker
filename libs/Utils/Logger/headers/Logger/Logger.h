#pragma once

#include <iostream>

namespace pla::logger {

/*!
 * @brief Class to log additional information
 */
class Logger
{
public:

  /*!
   * @brief Print information to sink.
   * [TODO]: Add ability to change sinks (game console logs, etc.)
   *
   * @param str Const reference for warning string.
   */
  static void printInfo(const std::string& str);

  /*!
   * @brief Debug print. It will become empty function in release.
   * @param str String that should be displayed in console.
   */
  static void printDebug(const std::string& str);
};

} // namespaces
