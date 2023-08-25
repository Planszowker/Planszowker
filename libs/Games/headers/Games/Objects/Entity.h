#pragma once

#include <Games/Objects/Object.h>

#include <SFML/Graphics.hpp>

namespace pla::games {

struct EntityFields : public ObjectFields{
  std::string texture;                      ///< Optional, default: ""
  std::string positionAsDestinationPoint;   ///< Optional, default: "Init"
  sf::Vector2f size;                        ///< Optional, default: (0.0, 0.0)
  bool visible;                             ///< Optional, default: false
  bool clickable;                           ///< Optional, default: false
};

class Entity : public Object {
public:
  explicit Entity(nlohmann::json json);

  ObjectFields* getParams() final { return &m_fields; }

  void setVisibility(bool visibility) final { m_fields.visible = visibility; }
  void updateTexture(const std::string& texture) { m_fields.texture = texture; }
private:
  EntityFields m_fields;
};

}