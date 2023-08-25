#include <Games/BoardParser.h>

#include <Games/Objects/ActionButton.h>
#include <Games/Objects/DestinationPoint.h>
#include <Games/Objects/Entity.h>
#include <Games/Objects/Tile.h>

#include <easylogging++.h>
#include <memory>

namespace pla::games {

using namespace board_entries;
using namespace json_entries;

BoardParser::BoardParser(nlohmann::json json)
  : m_boardJson(std::move(json))
{
  std::scoped_lock lock{m_mutex};
  // Parse ActionBar
  try {
    for (const auto& actionBarEntry : m_boardJson[ACTION_BAR]) {
      auto actionButton = std::make_shared<ActionButton>(actionBarEntry);
      std::string id = actionButton->getParams()->id;
      m_actionButtons.emplace(id, std::move(actionButton));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "This game does not contain any ActionBar entries!";
  }

  // Parse DestinationPoints
  try {
    // We add artificial `Init` Destination Point for these Entities that do not specify it.
    auto initDestinationPointJson = nlohmann::json::parse(R"({
      "ID": "Init",
      "Position": "0.0x0.0"
    })");
    m_boardJson[DESTINATION_POINTS].push_back(initDestinationPointJson);
    for (const auto& destinationPointEntry : m_boardJson[DESTINATION_POINTS]) {
      auto destinationPoint = std::make_shared<DestinationPoint>(destinationPointEntry);
      std::string id = destinationPoint->getParams()->id;
      m_destinationPoints.emplace(id, std::move(destinationPoint));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "This game does not contain any Destination entries!";
  }

  // Parse Entities
  try {
    for (const auto& entityEntry : m_boardJson[ENTITIES]) {
      auto entity = std::make_shared<Entity>(entityEntry);
      std::string id = entity->getParams()->id;
      m_entities.emplace(id, std::move(entity));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "This game does not contain any Entities entries!";
  }

  // Parse Tiles
  try {
    for (const auto& tileEntry : m_boardJson[TILES]) {
      auto tile = std::make_shared<Tile>(tileEntry);
      std::string id = tile->getParams()->id;
      m_tiles.emplace(id, std::move(tile));
    }
  } catch (std::exception& e) {
    LOG(INFO) << "This game does not contain any Tiles entries!";
  }
}


void BoardParser::updateObjects(const nlohmann::json& updateJson) {
  m_replyJson = updateJson;

  // Check if game has finished
  try {
    m_gameFinished = updateJson.at(GAME_FINISHED);
    if (m_gameFinished) {
      LOG(DEBUG) << "[BoardParser::updateObjects] Game has finished!";
    }
  } catch (std::exception& e) {
    LOG(DEBUG) << "[BoardParser::updateObjects] No information whether game has finished!";
  }

  // Check if we have informations about turn
  try {
    m_currentTurnClientId = updateJson.at(TURN_CLIENT_ID);
    LOG(DEBUG) << "[BoardParser::updateObjects] Current turn client ID: " << m_currentTurnClientId;
  } catch (std::exception& e) {
    LOG(DEBUG) << "[BoardParser::updateObjects] There were no information about turn client ID!";
  }

  // Check for any actions required
  try {
    auto actionEntries = updateJson.at(ACTIONS);
    for (const auto& actionEntry: actionEntries) {
      auto actionType = actionEntry.at(ACTION).get<std::string>();
      // LOG(DEBUG) << "[BoardParser::updateObjects] Handling " << actionType;
      if (actionType == ACTION_SET_TEXTURE) {
        // Setting new texture has to have these fields defined:
        //  > Entity - entity id to which new texture should be applied,
        //  > Texture - texture name (file name) to be applied.
        auto entity = actionEntry.at(ACTION_SET_TEXTURE_ENTITY_ID).get<std::string>();
        auto texture = actionEntry.at(ACTION_SET_TEXTURE_TEXTURE).get<std::string>();

        auto entityIt = m_entities.find(entity);
        if (entityIt != m_entities.end()) {
          std::dynamic_pointer_cast<Entity>(entityIt->second)->updateTexture(texture);
          _markBoardUpdate();
        }
      }
      else if (actionType == ACTION_SET_VISIBILITY) {
        // Setting object's visibility has to have these fields defined:
        //  > ObjectID - object's unique ID,
        //  > Visibility - boolean value whether the object should be visible.
        auto button = actionEntry.at(ACTION_SET_VISIBILITY_OBJECT_ID).get<std::string>();
        auto visible = actionEntry.at(ACTION_SET_VISIBILITY_VISIBILITY).get<bool>();

        auto buttonIt = m_actionButtons.find(button);
        if (buttonIt != m_actionButtons.end()) {
          std::dynamic_pointer_cast<ActionButton>(buttonIt->second)->setVisibility(visible);
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
    case UpdateActions::ObjectPressed:
      requestJson[ACTIONS].push_back({
        {ACTION, ACTION_OBJECT_PRESSED},
        {ACTION_OBJECT_PRESSED_OBJECT_ID, objectPtr->getParams()->id}
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