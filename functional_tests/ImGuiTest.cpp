#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>

int main()
{
  sf::RenderWindow window(sf::VideoMode(1080, 720), "ImGui + SFML test");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  ImGui::GetIO().IniFilename = nullptr;

  sf::Texture texture;
  if (not texture.loadFromFile("jan.png")) {
    return EXIT_FAILURE;
  }

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

    ImGui::SetNextWindowPos(ImVec2{400.f, 300.f}, ImGuiCond_FirstUseEver);
    ImGui::Begin("Hello, world!", nullptr);
    ImGui::Button("Look at this pretty button");
    ImGui::Button("Look at this pretty button");
    ImGui::Button("Look at this pretty button");
    ImGui::Image(texture, sf::Vector2f(100.f, 100.f));
    ImGui::Image(texture, sf::Vector2f(200.f, 200.f));
    ImGui::Image(texture, sf::Vector2f(300.f, 300.f));
    ImGui::End();

    window.clear();
    window.draw(shape);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();

  return 0;
}