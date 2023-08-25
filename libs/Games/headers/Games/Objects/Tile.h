#pragma once

#include <Games/Objects/Object.h>

#include <SFML/Graphics.hpp>

namespace pla::games {

struct AttachableSideAreaFields {
  std::string type;
  sf::Vector2f boundaryStartPos;
  sf::Vector2f boundaryEndPos;
};

struct TileFields : public ObjectFields {
  bool attachable;                                            ///< Optional, default: false
  sf::Vector2f size;                                          ///< Optional, default: (0.0, 0.0)
  bool rotable;                                               ///< Optional, default: false
  int initialRotate;                                          ///< Optionalm default: 0
  std::vector<AttachableSideAreaFields> attachableSideAreas;  ///< Optional, default: {}
};

class Tile : public Object {
public:
  explicit Tile(nlohmann::json json);

  ObjectFields* getParams() final { return &m_fields; }

  void setVisibility(bool) final { } // This method is not implemented in Tile object.
private:
  TileFields m_fields;
};

}