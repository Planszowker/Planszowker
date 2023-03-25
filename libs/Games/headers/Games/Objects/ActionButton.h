#pragma once

#include <Games/Objects/Object.h>

namespace pla::games {

struct ActionButtonFields {
  std::string type;
  std::string id;
  std::string displayName;
  bool visible {false};
};

class ActionButton : public Object {
public:
  explicit ActionButton(nlohmann::json json);

  ActionButtonFields getParams() { return m_fields; }
private:
  ActionButtonFields m_fields;
};

}