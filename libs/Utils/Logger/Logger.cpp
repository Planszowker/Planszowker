#include "Logger.h"

using namespace std;

namespace pla::logger {

void Logger::printInfo(const std::string& str)
{
  cout << str << "\n";
}


void Logger::printDebug(const std::string& str)
{
  cout << str << "\n";
}

} // namespaces
