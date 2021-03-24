#pragma once

/* STD */
#include <memory>
#include <any>

namespace pla::common::games {

/*!
 * @brief Class responsible for handling view logic, i.e. how to update Model
 * according to received data.
 */
class ViewLogic
{
public:

  /*!
   * @brief Update model with received data from Server.
   *
   * @param modelUpdateData Data received from Server. Game specific.
   */
  virtual void updateModel(const std::any& modelUpdateData) = 0;

  /*!
   * @brief Get updated model.
   *
   * @return Updated model object. Game specific.
   */
  virtual const std::any& getUpdatedModel() = 0;

protected:
};

} // namespaces
