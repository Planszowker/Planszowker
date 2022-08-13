#pragma once

namespace pla::games {

class ICallbacks
{
public:
  virtual ~ICallbacks() = default;

  virtual void IDCallback() = 0;
  virtual void listAllAvailableGamesCallback() = 0;
  // TODO: Increase callbacks if needed here...
protected:
  ICallbacks() = default;
};

}