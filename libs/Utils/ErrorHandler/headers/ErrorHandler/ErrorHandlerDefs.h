#pragma once

namespace pla::err_handler {

/*!
 *  \brief Enum class to hold priorities about thrown exceptions
 *
 *  Warning WILL NOT terminate program.\n
 *  Error WILL terminate program.
 */
enum class ExceptionPrio : unsigned int {
  Warning = 0,      ///< Warning prio
  Error = 1         ///< Critical error prio
};

} // namespaces