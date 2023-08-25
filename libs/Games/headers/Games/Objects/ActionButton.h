#pragma once

#include <Games/Objects/Object.h>

namespace pla::games {

struct ActionButtonFields : public ObjectFields {
  std::string type;
  std::string displayName;
  bool visible {false};
};

class ActionButton : public Object {
public:
  explicit ActionButton(nlohmann::json json);

  ObjectFields* getParams() final { return &m_fields; }

  void setVisibility(bool visibility) final { m_fields.visible = visibility; }
private:
  ActionButtonFields m_fields;
};

}