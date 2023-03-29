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
    m_fields.texture = m_json.at(TEXTURE).get<std::string>();
    m_fields.positionAsDestinationPoint = m_json.at(POSITION).get<std::string>();

    auto size = m_json.at(SIZE).get<std::string>();
    // Size is in format POS_X x POS_Y
    std::string delim = "x";
    auto delimPos = size.find(delim);
    m_fields.size.x = std::stof(size.substr(0, delimPos));
    m_fields.size.y = std::stof(size.substr(delimPos + delim.length()));

    LOG(DEBUG) << "Adding Entity with ID " << m_fields.id << ", texture (" << m_fields.texture << "), position as DestinationPoint " << m_fields.positionAsDestinationPoint << " and size " << m_fields.size.x << "x" << m_fields.size.y;
  } catch (std::exception& e) {
    LOG(WARNING) << "Error while parsing Entity! Check BoardDescription for any errors!";
  }
}

}