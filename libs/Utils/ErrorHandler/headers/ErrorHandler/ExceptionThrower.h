#pragma once

#include <exception>
#include <cstdlib>
#include <iostream>

#include "ErrorHandlerDefs.h"

namespace pla::common::err_handler {

/*!
 *  \brief Class to throw custom exceptions with different priorities
 */
class ExceptionThrower : public std::exception
{
public:

  /*!
   *  \brief Constructor
   *
   *  \param message Message string to be printed out
   *  \param prio Priority of a message
   *  \see ExceptionPrio
   */
  ExceptionThrower(const std::string& message, ExceptionPrio prio);

  /*!
   *  \brief Method to get priority of a thrown exception
   *
   *  \return Priority of a thrown exception
   */
  ExceptionPrio getPrio() noexcept;

  /*!
   *  \brief Method to get exception message
   *
   *  \return Message of a exception
   */
  std::string& getMessage() noexcept;

private:
  ExceptionPrio m_prio;
  std::string m_message;
};

} // namespaces
