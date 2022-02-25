#include "ErrorLogger.h"
#include "ExceptionThrower.h"

#include <iostream>

namespace pla::common::err_handler {

void ErrorLogger::printWarning(const std::string& str)
{
  throw ExceptionThrower(str, ExceptionPrio::Warning);
}


void ErrorLogger::printError(const std::string& str)
{
  throw ExceptionThrower(str, ExceptionPrio::Error);
}


void ErrorLogger::throwError()
{
  throw ExceptionThrower("", ExceptionPrio::Error);
}

} // namespaces
