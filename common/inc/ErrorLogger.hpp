#pragma once

#include <exception>
#include <cstdlib>
#include <iostream>


namespace Planszowker::common {

/*!
 *  \brief Enum class to hold priorities about thrown exceptions
 *
 *  Warning WILL NOT terminate program\n
 *  Error WILL terminate program
 */
enum class ExceptionPrio : unsigned int
{
  Warning = 0,      ///< Warning prio
  Error = 1         ///< Critical error prio
};


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
  ExceptionThrower(const std::string& message, const ExceptionPrio prio);

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


/*!
 *  \brief Class to log out warnings and error using exceptions
 *
 *  \see ExceptionThrower
 */
class ErrorLogger {
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

} // namespace
