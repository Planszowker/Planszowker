#include "ErrorLogger.h"
#include "ExceptionThrower.h"

#include <iostream>
#include <exception>

namespace pla::common::err_handler {

void ErrorLogger::printWarning(const std::string& str)
{
  throw ExceptionThrower(str, ExceptionPrio::Warning);
}


void ErrorLogger::printError(const std::string& str)
{
  throw ExceptionThrower(str, ExceptionPrio::Error);
}

} // namespaces
