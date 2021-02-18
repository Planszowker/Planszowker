#include "ExceptionThrower.h"

#include <iostream>
#include <exception>

namespace pla::common::err_handler {

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

} // namespaces