#pragma once

#include <Games/Objects/Object.h>

#include <SFML/Graphics.hpp>

namespace pla::games {

struct DestinationPointFields : public ObjectFields {
  sf::Vector2f position;
};

class DestinationPoint : public Object {
public:
  explicit DestinationPoint(nlohmann::json json);

  ObjectFields* getParams() final { return &m_fields; }

  void setVisibility(bool) final { } // This method is not implemented in DestinationPoint object.
private:
  DestinationPointFields m_fields;
};

}