#include <States/GameState.h>

#include <Callbacks/GameCallbacks.h>
#include <GamesClient/SharedObjects.h>
#include <Games/Objects.h>

namespace pla::games {

GameState::GameState(games_client::GraphicalView& graphicalView, GameStateArguments gameStateArguments)
  : m_graphicalView(graphicalView)
  , m_gameWindow(*graphicalView.getGameWindow())
  , m_controller(graphicalView.getController())
  , m_gameArguments(std::move(gameStateArguments))
  , m_callbacks(std::make_shared<GameCallbacks>())
//  , m_gameAreaWindow(sfg::Window::Create(sf::Style::None))
//  , m_playerAreaWindow(sfg::Window::Create(sf::Style::None))
//  , m_logAreaWindow(sfg::Window::Create(sf::Style::None))
//  , m_actionsAreaWindow(sfg::Window::Create(sf::Style::Fullscreen))
//  , m_gameAreaSfmlCanvas(sfg::Canvas::Create())
//  , m_playerAreaBox(sfg::Box::Create())
//  , m_logAreaBox(sfg::Box::Create())
//  , m_actionAreaBox(sfg::Box::Create())
{
  // Connect GameChoosing callbacks to packet handler
  m_controller.getPacketHandler()->connectCallbacks(m_callbacks.get());

  // Divide area into panels
  // _________________________________
  // |                      | PLAYER |  / \
  // |    (ACTIONS AREA)    |  AREA  |  0.6
  // |       GAME AREA      |--------|  ---
  // |                      |LOG AREA|  0.4
  // |______________________|________|  \ /
  // |<=========0.8========>|<==0.2=>|

  // Set dimensions for every area
  sf::Vector2f gameAreaDim {static_cast<float>(m_gameWindow.getSize().x) * GameAreaWidthFactor, static_cast<float>(m_gameWindow.getSize().y)};
  sf::Vector2f playersAreaDim {static_cast<float>(m_gameWindow.getSize().x) * PlayerAndLogAreaWidthFactor, static_cast<float>(m_gameWindow.getSize().y) * (1.f - LogAreaHeightFactor)};
  sf::Vector2f logAreaDim {static_cast<float>(m_gameWindow.getSize().x) * PlayerAndLogAreaWidthFactor, static_cast<float>(m_gameWindow.getSize().y) * LogAreaHeightFactor};

  m_gameAreaView.reset(sf::FloatRect(0.f, 0.f, gameAreaDim.x, gameAreaDim.y));
  m_gameAreaView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
  m_mainWindowView.reset(sf::FloatRect(0.f, 0.f, static_cast<float>(m_gameWindow.getSize().x), static_cast<float>(m_gameWindow.getSize().y)));
  m_mainWindowView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

//  m_gameAreaWindow->Add(m_gameAreaSfmlCanvas);
//  m_playerAreaWindow->Add(m_playerAreaBox);
//  m_logAreaWindow->Add(m_logAreaBox);
//  m_actionsAreaWindow->Add(m_actionAreaBox);
//
//  m_gameAreaSfmlCanvas->SetRequisition(sf::Vector2f(m_gameAreaWindow->GetClientRect().width, m_gameAreaWindow->GetClientRect().height));
//
//  m_gameAreaWindow->SetRequisition(sf::Vector2f(gameAreaDim.x, gameAreaDim.y));
//  m_playerAreaWindow->SetRequisition(sf::Vector2f(playersAreaDim.x, playersAreaDim.y));
//  m_playerAreaWindow->SetPosition(sf::Vector2f(gameAreaDim.x, 0.f));
//  m_logAreaWindow->SetRequisition(sf::Vector2f(logAreaDim.x, logAreaDim.y));
//  m_logAreaWindow->SetPosition(sf::Vector2f(gameAreaDim.x, playersAreaDim.y));
//
//  // Todo: change this magic numbers
//  m_actionsAreaWindow->SetRequisition(sf::Vector2f(100.f, 60.f));
//  m_actionsAreaWindow->SetPosition(sf::Vector2f((gameAreaDim.x - m_actionsAreaWindow->GetRequisition().x) / 2.f, 100.f));
//
//  // Set desktop properties
//  m_desktop.SetProperties(
//          "Button {"
//          " FontSize: 14;"
//          "}"
//  );
//
//  m_desktop.Add(m_gameAreaWindow);
//  m_desktop.Add(m_playerAreaWindow);
//  m_desktop.Add(m_logAreaWindow);
//  m_desktop.Add(m_actionsAreaWindow);
}


void GameState::init()
{
}


void GameState::eventHandling()
{
  // Process events
  sf::Event event{};
  while (m_gameWindow.pollEvent(event))
  {
    // Close window: exit
    if (event.type == sf::Event::Closed) {
      m_gameWindow.close();
      m_graphicalView.stop();
      break;
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
      sf::Packet packet;

      games::Request request {
        .type = PacketType::GameSpecificData,
      };

      packet << request;
      m_controller.getPacketHandler()->sendPacket(packet);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::J) {
      sf::Packet packet;

      games::Request request {
              .type = PacketType::DownloadAssets,
      };

      packet << request;
      m_controller.getPacketHandler()->sendPacket(packet);
    }

    // m_desktop.HandleEvent(event);
  }
}


void GameState::display()
{
  // Todo: This shape is necessary so SFGUI can use default view...
  sf::RectangleShape workaroundShape{sf::Vector2f(0.f, 0.f)};

//  m_desktop.Update(1.f / 60.f); // Todo: Any possible problems with this?
//  m_desktop.BringToFront(m_actionsAreaWindow);

  m_gameWindow.clear(sf::Color(0x54, 0x54, 0x54));

//  // Display SFML canvas
//  m_gameAreaSfmlCanvas->Bind();
//  m_gameAreaSfmlCanvas->Clear(sf::Color( 0xff, 0x54, 0x54, 80));
//
//  // Draw the SFML Sprite.
//  m_gameAreaSfmlCanvas->SetView(m_gameAreaView);
//  m_gameAreaSfmlCanvas->Draw(workaroundShape);
//
//  m_gameAreaSfmlCanvas->Display();
//  m_gameAreaSfmlCanvas->Unbind();

  // Hack to set to default view. Without rendering *anything* the view is not set...
  m_gameWindow.setView(m_gameWindow.getDefaultView());
  m_gameWindow.draw(workaroundShape);

//  m_sfgui.Display(m_gameWindow);
  m_gameWindow.display();
}

} // namespace
