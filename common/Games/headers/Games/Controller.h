#pragma once

#include "ConsoleView.h"

namespace pla::common::games {

class Controller
{
  void run() = 0;
  void updateModel() = 0;
  void updateView() = 0;
  void getUpdatedModel() = 0;
  void handleViewEvent(ConsoleView& view);
};



} // namespaces
