#pragma once

#include <nlohmann/json.hpp>
#include <Games/Objects/Object.h>
#include <NetworkHandler/ClientPacketHandler.h>

#include <unordered_map>
#include <mutex>

namespace pla::games {

// Relate to and only to BoardDescription.json
namespace board_entries {
auto constexpr BOARD = "Board";
auto constexpr DESTINATION_POINTS = "DestinationPoints";
auto constexpr ENTITIES = "Entities";
auto constexpr ACTION_BAR = "ActionBar";
auto constexpr TILES = "Tiles";

auto constexpr TEXTURE = "Texture";
auto constexpr TYPE = "Type";
auto constexpr ID = "ID";
auto constexpr DISPLAY_NAME = "DisplayName";
auto constexpr VISIBLE = "Visible";
auto constexpr CLICKABLE = "Clickable";
auto constexpr POSITION = "Position";
auto constexpr SIZE = "Size";
auto constexpr ATTACHABLE = "Attachable";
auto constexpr ROTABLE = "Rotable";
auto constexpr INITIAL_ROTATE = "InitialRotate";
auto constexpr ATTACHABLE_SIDE_AREAS = "AttachableSideAreas";
auto constexpr BOUNDARY = "Boundary";

// Boundary
auto constexpr BOUNDARY_X_START = "x_start";
auto constexpr BOUNDARY_Y_START = "y_start";
auto constexpr BOUNDARY_X_END = "x_end";
auto constexpr BOUNDARY_Y_END = "y_end";
}

enum class UpdateActions {
  ObjectPressed,
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

  /**
   * Get Action Buttons.
   *
   * @return Map with object's ID and shared pointer to object.
   */
  ObjectContainer getActionButtons() {
    std::scoped_lock lock{m_mutex};
    return m_actionButtons;
  };

  /**
   * Get Destination Points.
   *
   * @return Map with object's ID and shared pointer to object.
   */
  ObjectContainer getDestinationPoints() {
    std::scoped_lock lock{m_mutex};
    return m_destinationPoints;
  };

  /**
   * Get Entities.
   *
   * @return Map with object's ID and shared pointer to object.
   */
  ObjectContainer getEntities() {
    std::scoped_lock lock{m_mutex};
    return m_entities;
  };

  /**
   * Get Tiles.
   *
   * @return Map with object's ID and shared pointer to object.
   */
  ObjectContainer getTiles() {
    std::scoped_lock lock{m_mutex};
    return m_tiles;
  }

  /**
   * Mark that board's update has been consumed.
   */
  void markBoardUpdated() { m_boardUpdate = false; }

  /**
   * Check whether board has been updated.
   *
   * @return [bool] True if board has been updated, false otherwise.
   */
  bool isMarkedForUpdate() const { return m_boardUpdate; }

  /**
   * Get current ClientID that has the turn.
   *
   * @return ClientID with current turn.
   */
  size_t getCurrentTurnClientId() const { return m_currentTurnClientId; }

  /**
   * Check if game is finished.
   *
   * @return True if game is finished, false otherwise.
   */
  bool isGameFinished() const { return m_gameFinished; }

  /**
   * Get raw Reply JSON.
   *
   * @return Const reference to raw Reply JSON.
   */
  nlohmann::json& getReplyJson() { return m_replyJson; }

private:
  void _markBoardUpdate() { m_boardUpdate = true; }

  nlohmann::json m_boardJson;
  nlohmann::json m_replyJson;

  ObjectContainer m_actionButtons;
  ObjectContainer m_destinationPoints;
  ObjectContainer m_entities;
  ObjectContainer m_tiles;

  bool m_gameFinished {false};
  size_t m_currentTurnClientId {0};

  std::mutex m_mutex;

  bool m_boardUpdate { false }; ///< Indicates whether board has been updated (client might need to update his view).
};

}