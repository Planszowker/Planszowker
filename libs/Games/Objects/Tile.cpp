#include <Games/Objects/Tile.h>

#include <Games/BoardParser.h>

#include <easylogging++.h>
#include <nlohmann/json.hpp>

namespace pla::games {

using namespace board_entries;

Tile::Tile(nlohmann::json json)
        : Object(std::move(json))
{
  try{
    m_fields.id = m_json.at(ID).get<std::string>();
    m_fields.attachable = m_json.value(TEXTURE, false);
    m_fields.rotable = m_json.value(ROTABLE, false);
    m_fields.initialRotate = m_json.value(INITIAL_ROTATE, 0);

    auto size = m_json.value(SIZE, "0.0x0.0");
    // Size is in format POS_X x POS_Y
    std::string delim = "x";
    auto delimPos = size.find(delim);
    m_fields.size.x = std::stof(size.substr(0, delimPos));
    m_fields.size.y = std::stof(size.substr(delimPos + delim.length()));

    auto attachableSideAreas = m_json.value(ATTACHABLE_SIDE_AREAS, nlohmann::json::object());
    for (const auto attachableSideArea : attachableSideAreas) {
      AttachableSideAreaFields sideArea;
      sideArea.type = attachableSideArea.value(TYPE, "");
      auto boundary = attachableSideArea.at(BOUNDARY);
      sideArea.boundaryStartPos.x = std::stof(boundary.at(BOUNDARY_X_START).get<std::string>());
      sideArea.boundaryStartPos.y = std::stof(boundary.at(BOUNDARY_Y_START).get<std::string>());
      sideArea.boundaryEndPos.x = std::stof(boundary.at(BOUNDARY_X_END).get<std::string>());
      sideArea.boundaryEndPos.y = std::stof(boundary.at(BOUNDARY_Y_END).get<std::string>());

      m_fields.attachableSideAreas.push_back(sideArea);
    }

    LOG(DEBUG) << "Adding Tile with ID " << m_fields.id << ", attachable (" << m_fields.attachable << "), with size " << m_fields.size.x << "x" << m_fields.size.y << ", rotable (" << m_fields.rotable << ")";
  } catch (std::exception& e) {
    LOG(WARNING) << "Error while parsing Tile! Check BoardDescription for any errors!\n" << e.what();
  }
}

}
