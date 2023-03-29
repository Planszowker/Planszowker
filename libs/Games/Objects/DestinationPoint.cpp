#include <Games/Objects/DestinationPoint.h>

#include <Games/BoardParser.h>

#include <easylogging++.h>
#include <nlohmann/json.hpp>

namespace pla::games {

using namespace board_entries;

DestinationPoint::DestinationPoint(nlohmann::json json)
        : Object(std::move(json))
{
  try{
    m_fields.id = m_json.at(ID);
    auto position = m_json.at(POSITION).get<std::string>();
    // Position is in format POS_X x POS_Y
    std::string delim = "x";
    auto delimPos = position.find(delim);
    m_fields.position.x = std::stof(position.substr(0, delimPos));
    m_fields.position.y = std::stof(position.substr(delimPos + delim.length()));

    LOG(DEBUG) << "Adding DestinationPoint with ID " << m_fields.id << " and position " << m_fields.position.x << " x " << m_fields.position.y;
  } catch (std::exception& e) {
    LOG(WARNING) << "Error while parsing DestinationPoint! Check BoardDescription for any errors!";
  }
}

}