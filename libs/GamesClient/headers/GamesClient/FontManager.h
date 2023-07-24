#pragma once

#include <imgui.h>
#include <imgui-SFML.h>
#include <unordered_map>

namespace pla::games_client {

class FontManager
{
public:
  void loadFonts();

  ImFont* getFont(const std::string& key) const;

private:
  std::unordered_map<std::string, ImFont*> m_fontAtlas;
};

} // namespaces