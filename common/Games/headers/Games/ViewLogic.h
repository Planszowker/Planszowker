#pragma once

#include "GenericView.h"

#include <memory>
#include <any>

namespace pla::common::games {

class GenericView;

/*!
 * @brief ViewLogic class. It loads specific LUA script that describes
 * how data should be handled.
 */
class ViewLogic
{
public:
  /*!
   * @brief Updates ViewLogic.
   * @param object Any object that certain game wants to send.
   * @see ViewLogic
   */
  virtual void update(std::any& object) = 0;

  /*!
   * @brief Update connected view with specific data.
   * @see GenericView
   */
  virtual void updateView() = 0;

protected:
  std::unique_ptr<GenericView> m_view;
};

} // namespaces
