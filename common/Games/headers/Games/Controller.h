#pragma once

#include "ConsoleView.h"

#include <any>

namespace pla::common::games {

class ConsoleView;

class Controller
{
public:
  virtual void run() = 0;
  virtual void attachView(ConsoleView* view) = 0;
  virtual void viewCallback(std::any object) = 0;

private:
  virtual void updateView() = 0;

  virtual void updateModel() = 0;
  virtual void getUpdatedModel() = 0;
};

} // namespaces
