#pragma once

#include <nlohmann/json.hpp>

namespace pla::games {

struct ObjectFields {
  std::string id;                                             ///< Obligatory
};

class Object {
public:
  explicit Object(nlohmann::json json)
    : m_json(std::move(json))
    {
    }

  virtual ~Object() = default;

  virtual ObjectFields* getParams() = 0;

  virtual void setVisibility(bool) = 0;
protected:
  nlohmann::json m_json;
};

}