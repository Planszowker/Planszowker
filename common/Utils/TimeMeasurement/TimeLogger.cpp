// TODO: Fix this weird issue
#include "Utils/TimeMeasurement/headers/TimeMeasurement/TimeLogger.h"

#include <chrono>
#include <iostream>

namespace pla::common::time_measurement{

using namespace pla::common;

TimeLogger::TimeLogger(std::string functionName)
  : m_tStart(std::chrono::high_resolution_clock::now())
  , m_functionName(std::move(functionName))
{
}

// NOTE: '\033' - escape, then '[', then numbers separated by ;, then 'm'
TimeLogger::~TimeLogger()
{
  auto t_end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - m_tStart).count();
  if (duration > 100) {
    std::cout << "\033[33;1m*TIME MEASUREMENT* \033[0;33m" << m_functionName << ": " << duration << " [ms].\033[0m\n";
  }
}

}