#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>

int main()
{
  sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML test");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  ImGui::GetIO().IniFilename = nullptr;

  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  sf::Clock deltaClock;
  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::SetCursorPos(ImVec2{200.f, 300.f});
    ImGui::Begin("Hello, world!", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
    ImGui::Button("Look at this pretty button");
    ImGui::Button("Look at this pretty button");
    ImGui::Button("Look at this pretty button");
    ImGui::End();

    window.clear();
    window.draw(shape);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}