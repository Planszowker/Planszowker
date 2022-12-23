#include <States/GameChoosingState.h>

#include <Games/Callbacks/GameChoosingCallbacks.h>

#include <easylogging++.h>

namespace pla::games {

GameChoosingState::GameChoosingState(games_client::GraphicalView& graphicalView)
  : m_graphicalView(graphicalView)
  , m_gameWindow(*graphicalView.getGameWindow())
  , m_controller(graphicalView.getController())
  , m_callbacks(std::make_shared<GameChoosingCallbacks>(*this))
  , m_gameChoosingWindow(sfg::Window::Create())
  , m_gamesEntriesBox(sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 20.f))
{
  // Connect GameChoosing callbacks to packet handler
  m_controller.getPacketHandler()->connectCallbacks(m_callbacks.get());

  // Set desktop properties
  m_desktop.SetProperties(
          "Button {"
          " FontSize: 14;"
          "}"
  );

  m_desktop.Add(m_gameChoosingWindow);
  m_gameChoosingWindow->SetRequisition(sf::Vector2f(400.f, 300.f));
  //m_gameChoosingWindow->SetStyle(sfg::Window::Style::NO_STYLE);
  m_gameChoosingWindow->Add(m_gamesEntriesBox);
}


void GameChoosingState::init()
{
  m_controller.sendRequest(games::PacketType::ID);
  m_controller.sendRequest(games::PacketType::ListAvailableGames);
}


void GameChoosingState::eventHandling()
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

    // TODO: Remove it later
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
      m_gamesMetaInfo.clear();

      m_controller.sendRequest(games::PacketType::ListAvailableGames);
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::H) {
      LOG(DEBUG) << "Content of m_gamesMetaInfo:";
      for (const auto& ziu : m_gamesMetaInfo.getPlametaParsers()) {
        LOG(DEBUG) << ziu.first;
      }
    }

    m_desktop.HandleEvent(event);
  }
}


void GameChoosingState::display()
{
  m_gameWindow.clear(sf::Color(55, 55, 55));

  m_desktop.Update(1.f / 60.f); // Todo: Any possible problems with this?
  m_desktop.BringToFront(m_gameChoosingWindow);

  if (m_updateMetaInfo) {
    const auto& plametas = m_gamesMetaInfo.getPlametaParsers();
    for (const auto &plameta: plametas) {
      m_gamesEntriesBox->Pack(this->_getGameEntryBox(plameta.first));
    }

    m_updateMetaInfo = false;
  }

  const auto& plametas = m_gamesMetaInfo.getPlametaParsers();
  const auto& thumbnails = m_gamesMetaInfo.getThumbnails();

  for (const auto& plameta : plametas) {
    const auto& key = plameta.first;

    sf::Sprite gameImageSprite;
    auto thumbnailIter = thumbnails.find(key);
    if (thumbnailIter == thumbnails.end()) {
      // There's no custom thumbnail for given game - use the default one
      // TODO: This hard-coded file name is not the best idea...
      gameImageSprite.setTexture(*thumbnails.find("DefaultThumbnail")->second);
    } else {
      // There's a custom thumbnail for given name - use it
      gameImageSprite.setTexture(*thumbnailIter->second);
    }

    if (gameImageSprite.getTexture()) {
//      gameImageSprite.setScale(ThumbnailSize.x / gameImageSprite.getTexture()->getSize().x,
//                                ThumbnailSize.y / gameImageSprite.getTexture()->getSize().y);

      auto canvas = std::reinterpret_pointer_cast<sfg::Canvas>(sfg::Widget::GetWidgetById(key));
      if (canvas) {
        canvas->Bind();
        canvas->Clear(sf::Color::White);
        canvas->Draw(gameImageSprite);
        canvas->Display();
        canvas->Unbind();
      }
    }
  }

  m_gameWindow.setActive();
  m_sfgui.Display(m_gameWindow);
  m_gameWindow.display();
}


void GameChoosingState::updateAvailableGames(const std::string& combinedString)
{
  m_gamesMetaInfo.addMetaData(combinedString);
  m_updateMetaInfo = true;
}


sfg::Box::Ptr GameChoosingState::_getGameEntryBox(const std::string& key)
{
  sfg::Box::Ptr gameEntryBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 0.1f);

  const auto& plametas = m_gamesMetaInfo.getPlametaParsers();

  auto& parser = plametas.find(key)->second;
  auto gameNameLabel = sfg::Label::Create(std::get<std::string>(parser["overview:name"]->getVariant()));

  auto gameImage = sfg::Canvas::Create();
  gameImage->SetRequisition(sf::Vector2f(200.f, 200.f));
  gameImage->SetId(key);

//  auto buttonsBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 20.f);
//  auto playButton = sfg::Button::Create("Play");
//  auto infoButton = sfg::Button::Create("Info");

  //buttonsBox->Pack(playButton);
  //buttonsBox->Pack(infoButton);

  // Pack label, image and play button into box
  gameEntryBox->Pack(gameNameLabel);
  gameEntryBox->Pack(gameImage);
  //gameEntryBox->Pack(buttonsBox);

  return gameEntryBox;
}

} // namespace
