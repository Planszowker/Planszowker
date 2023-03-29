#include <Games/BoardParser.h>

#include <Games/Objects/ActionButton.h>
#include <Games/Objects/DestinationPoint.h>
#include <Games/Objects/Entity.h>

#include <easylogging++.h>

namespace pla::games {

using namespace board_entries;

BoardParser::BoardParser(nlohmann::json json)
  : m_json(std::move(json))
{
  // Parse ActionBar
  try {
    for (const auto& actionBarEntry : m_json[ACTION_BAR]) {
      auto actionButton = std::make_shared<ActionButton>(actionBarEntry);
      std::string id = actionButton->getParams().id;
      m_actionButtons.emplace(id, std::move(actionButton));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "This game does not contain any ActionBar entries!";
  }

  // Parse DestinationPoints
  try {
    for (const auto& destinationPointEntry : m_json[DESTINATION_POINTS]) {
      auto destinationPoint = std::make_shared<DestinationPoint>(destinationPointEntry);
      std::string id = destinationPoint->getParams().id;
      m_destinationPoints.emplace(id, std::move(destinationPoint));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "This game does not contain any Destination entries!";
  }

  // Parse Entities
  try {
    for (const auto& entityEntry : m_json[ENTITIES]) {
      auto entity = std::make_shared<Entity>(entityEntry);
      std::string id = entity->getParams().id;
      m_destinationPoints.emplace(id, std::move(entity));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "This game does not contain any Entities entries!";
  }
}


void BoardParser::updateObjects(nlohmann::json updateJson) {
  // TODO
}

} // namespace