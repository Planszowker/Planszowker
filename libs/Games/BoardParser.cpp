#include <Games/BoardParser.h>

#include <Games/Objects/ActionButton.h>
#include <Games/Objects/DestinationPoint.h>
#include <Games/Objects/Entity.h>

#include <easylogging++.h>
#include <memory>

namespace pla::games {

using namespace board_entries;
using namespace json_entries;

BoardParser::BoardParser(nlohmann::json json)
  : m_json(std::move(json))
{
  std::scoped_lock lock{m_mutex};
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
      m_entities.emplace(id, std::move(entity));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "This game does not contain any Entities entries!";
  }
}


void BoardParser::updateObjects(const nlohmann::json& updateJson) {
  // Check for any actions required
  try {
    auto actionEntries = updateJson.at(ACTIONS);
    for (const auto& actionEntry: actionEntries) {
      auto actionType = actionEntry.at(ACTION).get<std::string>();
      if (actionType == ACTION_SET_TEXTURE) {
        // Setting new texture has to have these fields defined:
        //  > Entity - entity id to which new texture should be applied
        //  > Texture - texture name (file name) to be applied
        auto entity = actionEntry.at(ACTION_ENTITY_ID).get<std::string>();
        auto texture = actionEntry.at(ACTION_TEXTURE).get<std::string>();

        auto entityIt = m_entities.find(entity);
        if (entityIt != m_entities.end()) {
          std::dynamic_pointer_cast<Entity>(entityIt->second)->updateTexture(texture);
          _markBoardUpdate();
        }
      }
    }
  } catch (std::exception& e) {
    LOG(DEBUG) << "[BoardParser::updateObjects] No actions to be performed!";
  }
}


void BoardParser::performUpdateAndSendToServer(network::ClientPacketHandler& packetHandler, const std::shared_ptr<Object>& objectPtr, UpdateActions updateAction)
{
  if (not objectPtr) {
    return;
  }

  nlohmann::json requestJson;

  switch (updateAction) {
    case UpdateActions::ButtonPressed:
      auto buttonPtr = dynamic_pointer_cast<ActionButton>(objectPtr);
      auto params = buttonPtr->getParams();
      requestJson[ACTIONS].push_back({
        {ACTION, BUTTON_PRESSED_UPDATE},
        {INFO, params.id}
      });
      break;
  }

  sf::Packet packet;
  Request request {
    .type = PacketType::GameSpecificData,
    .body = requestJson.dump(),
  };
  packet << request;
  packetHandler.sendPacket(packet);
}

} // namespace