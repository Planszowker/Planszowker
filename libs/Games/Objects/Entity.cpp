#include <Games/Objects/Entity.h>

#include <Games/BoardParser.h>
#include <AssetsManager/AssetsReceiver.h>

#include <easylogging++.h>
#include <nlohmann/json.hpp>

namespace pla::games {

using namespace assets;
using namespace board_entries;

Entity::Entity(nlohmann::json json)
        : Object(std::move(json))
{
  try{
    m_fields.id = m_json.at(ID).get<std::string>();
    m_fields.texture = m_json.value(TEXTURE, "");
    m_fields.positionAsDestinationPoint = m_json.value(POSITION, "Init"); // There's an `Init` destination point added artificially
    m_fields.visible = m_json.value(VISIBLE, false);
    m_fields.clickable = m_json.value(CLICKABLE, false);

    auto size = m_json.value(SIZE, "0.0x0.0");
    // Size is in format POS_X x POS_Y
    std::string delim = "x";
    auto delimPos = size.find(delim);
    m_fields.size.x = std::stof(size.substr(0, delimPos));
    m_fields.size.y = std::stof(size.substr(delimPos + delim.length()));

    LOG(DEBUG) << "Adding Entity with ID " << m_fields.id << ", texture (" << m_fields.texture << "), position as DestinationPoint " << m_fields.positionAsDestinationPoint << " and size " << m_fields.size.x << "x" << m_fields.size.y;
  } catch (std::exception& e) {
    LOG(WARNING) << "Error while parsing Entity! Check BoardDescription for any errors! " << e.what();
  }
}

}
