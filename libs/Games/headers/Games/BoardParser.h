#pragma once

#include <nlohmann/json.hpp>
#include <Games/Objects/Object.h>
#include <NetworkHandler/ClientPacketHandler.h>

#include <unordered_map>
#include <mutex>

namespace pla::games {

namespace board_entries {
auto constexpr BOARD = "Board";
auto constexpr DESTINATION_POINTS = "DestinationPoints";
auto constexpr ENTITIES = "Entities";
auto constexpr ACTION_BAR = "ActionBar";

auto constexpr TEXTURE = "Texture";
auto constexpr TYPE = "Type";
auto constexpr ID = "ID";
auto constexpr DISPLAY_NAME = "DisplayName";
auto constexpr VISIBLE = "Visible";
auto constexpr POSITION = "Position";
auto constexpr SIZE = "Size";

// Update
auto constexpr BUTTON_PRESSED_UPDATE = "ButtonPressed";

// Set texture update request
auto constexpr ACTION_SET_TEXTURE = "SetTexture";
auto constexpr ACTION_ENTITY_ID = "Entity";
auto constexpr ACTION_TEXTURE = "Texture";

// Set object's visibility
auto constexpr ACTION_SET_VISIBILITY = "SetVisibility";
auto constexpr ACTION_OBJECT_ID = "ObjectID";
auto constexpr ACTION_VISIBILITY = "Visibility";
}

enum class UpdateActions {
  ButtonPressed,
};

class BoardParser {
public:
  using ObjectContainer = std::unordered_map<std::string, std::shared_ptr<Object>>;

  explicit BoardParser(nlohmann::json json);

  /**
   * Update internal objects based on received reply.
   *
   * @param updateJson JSON with update entries
   */
  void updateObjects(const nlohmann::json& updateJson);

  /**
   * Update internal object based on user actions and send them to server.
   *
   * @param packetHandler Packet handler to be used to send request to server.
   * @param objectPtr Object pointer.
   * @param updateAction Action that was performed by user.
   */
  void performUpdateAndSendToServer(network::ClientPacketHandler& packetHandler, const std::shared_ptr<Object>& objectPtr, UpdateActions updateAction);

  ObjectContainer getActionButtons() {
    std::scoped_lock lock{m_mutex};
    return m_actionButtons;
  };

  ObjectContainer getDestinationPoints() {
    std::scoped_lock lock{m_mutex};
    return m_destinationPoints;
  };

  ObjectContainer getEntities() {
    std::scoped_lock lock{m_mutex};
    return m_entities;
  };

  void markBoardUpdated() { m_boardUpdate = false; }
  bool isMarkedForUpdate() { return m_boardUpdate; }

  size_t getCurrentTurnClientId() { return m_currentTurnClientId; }
  bool isGameFinished() { return m_gameFinished; }

private:
  void _markBoardUpdate() { m_boardUpdate = true; }

  nlohmann::json m_json;

  ObjectContainer m_actionButtons;
  ObjectContainer m_destinationPoints;
  ObjectContainer m_entities;

  bool m_gameFinished {false};
  size_t m_currentTurnClientId {0};

  std::mutex m_mutex;

  bool m_boardUpdate { false };
};

}