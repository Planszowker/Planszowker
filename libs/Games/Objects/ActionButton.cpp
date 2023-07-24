#include <Games/Objects/ActionButton.h>

#include <Games/BoardParser.h>

#include <easylogging++.h>
#include <nlohmann/json.hpp>

namespace pla::games {

using namespace board_entries;

ActionButton::ActionButton(nlohmann::json json)
  : Object(std::move(json))
{
  try{
    m_fields.type = m_json.at(TYPE);
    m_fields.id = m_json.at(ID);
    m_fields.displayName = m_json.at(DISPLAY_NAME);
    m_fields.visible = m_json.at(VISIBLE);

    LOG(DEBUG) << "Adding ActionButton with ID " << m_fields.id << " and DisplayName " << m_fields.displayName << " (default visibility: " << m_fields.visible << ")";
  } catch (std::exception& e) {
    LOG(WARNING) << "Error while parsing ActionBar! Check BoardDescription for any errors!";
  }
}

}