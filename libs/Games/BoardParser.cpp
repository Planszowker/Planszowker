#include <Games/BoardParser.h>

#include <Games/Objects/ActionButton.h>
#include <Games/Objects/DestinationPoint.h>
#include <Games/Objects/Entity.h>

#include <easylogging++.h>

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


void BoardParser::updateObjects(nlohmann::json updateJson) {
  // TODO
}


void BoardParser::performUpdateAndSendToServer(network::ClientPacketHandler& packetHandler, const std::shared_ptr<Object>& objectPtr, UpdateActions updateAction)
{
  if (not objectPtr) {
    return;
  }

  nlohmann::json request_json;

  switch (updateAction) {
    case UpdateActions::ButtonPressed:
      auto buttonPtr = dynamic_pointer_cast<ActionButton>(objectPtr);
      auto params = buttonPtr->getParams();
      request_json[ACTION] = BUTTON_PRESSED_UPDATE;
      request_json[OBJECTS].push_back({
        {ID, params.id},
      });
      break;
  }

  sf::Packet packet;
  Request request {
    .type = PacketType::GameSpecificData,
    .body = request_json.dump(),
  };
  packet << request;
  packetHandler.sendPacket(packet);
}

} // namespace