#pragma once

#include <chrono>
#include <iostream>

namespace pla::common::time_measurement {

class TimeLogger{
public:
  explicit TimeLogger(std::string functionName);
  ~TimeLogger();

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_tStart;
  std::string m_functionName;

  // TODO: Add file-logger support
};

}