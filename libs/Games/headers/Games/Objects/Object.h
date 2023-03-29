#pragma once

#include <nlohmann/json.hpp>

namespace pla::games {

class Object {
public:
  explicit Object(nlohmann::json json)
    : m_json(std::move(json))
    {
    }

  virtual ~Object() = default;
protected:
  nlohmann::json m_json;
};

}