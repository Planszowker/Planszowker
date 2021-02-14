#include "ErrorLogger.h"

#include <iostream>
#include <exception>

using namespace pla::common::err_logger;


ExceptionThrower::ExceptionThrower(const std::string& message, const ExceptionPrio prio)
{
  m_prio = prio;
  m_message = message;
}


ExceptionPrio ExceptionThrower::getPrio() noexcept
{
  return m_prio;
}


std::string& ExceptionThrower::getMessage() noexcept
{
  return m_message;
}


void ErrorLogger::printWarning(const std::string& str)
{
  throw ExceptionThrower(str, ExceptionPrio::Warning);
}


void ErrorLogger::printError(const std::string& str)
{
  throw ExceptionThrower(str, ExceptionPrio::Error);
}


} // namespace
