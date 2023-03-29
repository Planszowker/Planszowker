#pragma once

#include <Games/Objects/Object.h>

#include <SFML/Graphics.hpp>

namespace pla::games {

struct DestinationPointFields {
  std::string id;
  sf::Vector2f position;
};

class DestinationPoint : public Object {
public:
  explicit DestinationPoint(nlohmann::json json);

  DestinationPointFields getParams() { return m_fields; }
private:
  DestinationPointFields m_fields;
};

}