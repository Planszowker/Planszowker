#pragma once

#include <Games/Objects/Object.h>

#include <SFML/Graphics.hpp>

namespace pla::games {

struct EntityFields {
  std::string id;
  std::string texture;
  std::string positionAsDestinationPoint;
  sf::Vector2f size;
  bool visible;
};

class Entity : public Object {
public:
  explicit Entity(nlohmann::json json);

  EntityFields getParams() { return m_fields; }
private:
  EntityFields m_fields;
};

}