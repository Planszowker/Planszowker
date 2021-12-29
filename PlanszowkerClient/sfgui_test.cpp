#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RendererViewport.hpp>

////////////////////////////
// Testing SFGUI and SFML //
////////////////////////////
int main()
{
  sf::RenderWindow window{sf::VideoMode(1280U, 720U), "Testing SFGUI and SFML"};
  sfg::SFGUI sfgui;

  sf::View mainView;
  mainView.reset(sf::FloatRect(0.f, 0.f, 1280.f, 720.f));
  mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

  auto leftWindow = sfg::Window::Create();
  auto rightWindow = sfg::Window::Create();
  auto actionWindow = sfg::Window::Create();

  leftWindow->SetRequisition(sf::Vector2f(1000.f, 720.f));
  rightWindow->SetRequisition(sf::Vector2f(280.f, 720.f));
  rightWindow->SetPosition(sf::Vector2f(1000.f, 0.f));
  actionWindow->SetRequisition(sf::Vector2f(400.f, 0.f));
  actionWindow->SetPosition(sf::Vector2f(300.f, 70.f));

  auto boxRight = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 8.f);

  auto boxAction = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 8.f);
  auto actionButton1 = sfg::Button::Create("Something");
  auto actionButton2 = sfg::Button::Create("Another thing");

  auto canvas = sfg::Canvas::Create();

  sf::Texture texture;
  texture.loadFromFile("map.png");
  sf::Sprite mapSprite;
  mapSprite.setTexture(texture);

  sfg::Desktop desktop;
  desktop.SetProperties(
          "Button {"
          " FontSize: 14;"
          "}"
  );

  sfg::Button::Ptr exampleButtons[2] = {
          sfg::Button::Create("Ziuziuruziu"),
          sfg::Button::Create("Fufu w ruchu")
  };

  for (const auto& button: exampleButtons) {
    boxRight->Pack(button, false);
  }

  boxAction->Pack(actionButton1);
  boxAction->Pack(actionButton2);

  leftWindow->Add(canvas);
  rightWindow->Add(boxRight);
  //actionWindow->Add(boxAction);

  desktop.Add(leftWindow);
  desktop.Add(rightWindow);
  desktop.Add(actionWindow);

  window.resetGLStates();
  window.setFramerateLimit(60.f);

  sf::CircleShape circle{20.f};

  sf::RectangleShape totallyUnnecessarryRectangleThatShouldNotExist {sf::Vector2f(0.f, 0.f)};

  sf::View mapView;
  std::cout << "Allocation: " << leftWindow->GetAllocation().width << " " << leftWindow->GetAllocation().height << "\n";
  //mapView.setSize(leftWindow->GetAllocation().width, leftWindow->GetAllocation().height);
  //mapView.setCenter(leftWindow->GetAllocation().width / 2.f, leftWindow->GetAllocation().height / 2.f);
  mapView.reset(sf::FloatRect(0.f, 0.f, 595.f, 595.f));
  mapView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

  actionWindow->GetSignal(sfg::Window::OnLostFocus).Connect([&desktop, &actionWindow]{
    desktop.BringToFront(actionWindow);
  });

  leftWindow->GetSignal(sfg::Window::OnGainFocus).Connect([&desktop, &actionWindow]{

  });

  while (window.isOpen())
  {
    sf::Event event{};
    while (window.pollEvent(event))
    {
      if (sf::Event::EventType::Closed == event.type) {
        window.close();
        break;
      }

      /*if (sf::Event::EventType::Resized == event.type) {
        leftWindow->SetAllocation(sf::FloatRect(0.f, 0.f, 0.78125f * static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
        rightWindow->SetAllocation(sf::FloatRect(leftWindow->GetAllocation().width, 0.f, (1.f - 0.78125f) * static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
        actionWindow->SetPosition(sf::Vector2f((leftWindow->GetAllocation().width - 400.f) / 2.f, 70.f));
      }*/

      desktop.HandleEvent(event);

    }
    desktop.Update(1.0f / 60.f);

    window.clear(sf::Color(0x54, 0x54, 0x54));

    canvas->Bind();
    canvas->Clear( sf::Color( 0, 0, 0, 0 ) );

    // Draw the SFML Sprite.
    canvas->SetView(mapView);
    canvas->Draw(mapSprite);

    canvas->Display();
    canvas->Unbind();

    // Ugly AF hack
    window.setView(mainView);
    window.draw(totallyUnnecessarryRectangleThatShouldNotExist);


    sfgui.Display(window);
    window.display();
  }

  return EXIT_SUCCESS;
}
