#pragma once

namespace pla::common::games {

class ServerLogic
{
public:
  virtual void run() = 0;
  virtual void stop() = 0;
};

} // namespaces
