#include "FontManager.h"

#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>

namespace pla::games_client {

void FontManager::loadFonts()
{
  std::vector<std::tuple<std::string, float, std::string>> fontEntries;
  fontEntries.emplace_back("data/fonts/Roboto-Light.ttf", 32.f, "Roboto-Light-32px");
  fontEntries.emplace_back("data/fonts/Roboto-Light.ttf", 24.f, "Roboto-Light-24px");
  fontEntries.emplace_back("data/fonts/Roboto-Light.ttf", 18.f, "Roboto-Light-18px");

  auto& io = ImGui::GetIO();

  // Load font atlas
  for (const auto& fontEntry : fontEntries) {
    ImFont* font = io.Fonts->AddFontFromFileTTF(std::get<0>(fontEntry).c_str(), std::get<1>(fontEntry));
    m_fontAtlas.insert({std::get<2>(fontEntry), font});
  }

  ImGui::SFML::UpdateFontTexture();

  //io.Fonts->Build();
}


ImFont* FontManager::getFont(const std::string& key) const
{
  auto fontIter = m_fontAtlas.find(key);
  if(fontIter != m_fontAtlas.end()) {
    return fontIter->second;
  }
  return nullptr;
}

} // namespaces