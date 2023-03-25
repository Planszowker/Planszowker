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
}

class BoardParser {
public:
  explicit BoardParser(nlohmann::json json);
private:
  using ObjectContainer = std::unordered_map<std::string, std::shared_ptr<Object>>;

  nlohmann::json m_json;

  ObjectContainer m_actionButtons;
  ObjectContainer m_destinationPoints;
  ObjectContainer m_entities;
};

}