#pragma once

#include <vector>
#include <string>

namespace pla::common::games {

/*!
 * @brief Object that has all necessary data requested by a view.
 */
class ViewObject {
public:

  /*!
   * @brief Get strings that have to be displayed in game's log console.
   * @return Vector that holds desired strings.
   */
  std::vector<std::string>& getConsoleOutput();

  /*!
   * @brief Gets game's special structure that hold information about players and game update.
   * @return Void pointer to struct with update info. Needs recasting.
   */
  void* getGameUpdate();
private:
};

} // namespaces