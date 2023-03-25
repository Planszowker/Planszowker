#include <Games/BoardParser.h>

#include <Games/Objects/ActionButton.h>

#include <easylogging++.h>

namespace pla::games {

using namespace board_entries;

BoardParser::BoardParser(nlohmann::json json)
  : m_json(std::move(json))
{
  try {
    for (const auto& actionBarEntry : m_json[ACTION_BAR]) {
      auto actionButton = std::make_shared<ActionButton>(actionBarEntry);
      std::string id = actionButton->getParams().id;
      m_actionButtons.emplace(id, std::move(actionButton));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "This game does not contain any ActionBar entries!";
  }
}

} // namespace