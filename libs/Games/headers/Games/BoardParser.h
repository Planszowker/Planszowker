#pragma once

#include <nlohmann/json.hpp>
#include <Games/Objects/Object.h>

#include <unordered_map>

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
}

class BoardParser {
public:
  using ObjectContainer = std::unordered_map<std::string, std::shared_ptr<Object>>;

  explicit BoardParser(nlohmann::json json);

  void updateObjects(nlohmann::json updateJson);

  ObjectContainer& getActionButtons() { return m_actionButtons; };
  ObjectContainer& getDestinationPoints() { return m_destinationPoints; };
  ObjectContainer& getEntities() { return m_entities; };

private:
  nlohmann::json m_json;

  ObjectContainer m_actionButtons;
  ObjectContainer m_destinationPoints;
  ObjectContainer m_entities;
};

}