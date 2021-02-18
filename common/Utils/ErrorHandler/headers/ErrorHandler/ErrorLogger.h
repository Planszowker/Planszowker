#pragma once

#include <exception>
#include <cstdlib>
#include <iostream>

#include "ErrorHandlerDefs.h"

namespace pla::common::err_handler {

/*!
 *  \brief Class to log out warnings and error using exceptions
 *
 *  \see ExceptionThrower
 */
class ErrorLogger
{
public:

  /*!
   *  \brief Print warning and continue execution.
   *
   *  \param str Const reference for warning string.
   */
  void printWarning(const std::string& str);


  /*!
   *  \brief Print error and stop execution.
   *
   *  \param str Const reference for error string.
   */
  void printError(const std::string& str);
};

} // namespaces
