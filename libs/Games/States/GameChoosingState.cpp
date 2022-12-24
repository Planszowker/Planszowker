#include <States/GameChoosingState.h>

#include <Games/Callbacks/GameChoosingCallbacks.h>

#include <easylogging++.h>

namespace pla::games {

GameChoosingState::GameChoosingState(games_client::GraphicalView& graphicalView)
  : m_graphicalView(graphicalView)
  , m_gameWindow(*graphicalView.getGameWindow())
  , m_controller(graphicalView.getController())
  , m_callbacks(std::make_shared<GameChoosingCallbacks>(*this))
{
  // Connect GameChoosing callbacks to packet handler
  m_controller.getPacketHandler()->connectCallbacks(m_callbacks.get());
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
  }
}


void GameChoosingState::display()
{
  m_gameWindow.clear(sf::Color(55, 55, 55));


  if (m_updateMetaInfo) {
    const auto& plametas = m_gamesMetaInfo.getPlametaParsers();
    for (const auto &plameta: plametas) {
      //m_gamesEntriesBox->Pack(this->_getGameEntryBox(plameta.first));
    }

    m_updateMetaInfo = false;
  }

  const auto& plametas = m_gamesMetaInfo.getPlametaParsers();
  const auto& thumbnails = m_gamesMetaInfo.getThumbnails();

  for (const auto& plameta : plametas) {
    const auto& key = plameta.first;

//    sf::Sprite gameImageSprite;
//    auto thumbnailIter = thumbnails.find(key);
//    if (thumbnailIter == thumbnails.end()) {
//      // There's no custom thumbnail for given game - use the default one
//      // TODO: This hard-coded file name is not the best idea...
//      gameImageSprite.setTexture(*thumbnails.find("DefaultThumbnail")->second);
//    } else {
//      // There's a custom thumbnail for given name - use it
//      gameImageSprite.setTexture(*thumbnailIter->second);
//    }
//
//    if (gameImageSprite.getTexture()) {
////      gameImageSprite.setScale(ThumbnailSize.x / gameImageSprite.getTexture()->getSize().x,
////                                ThumbnailSize.y / gameImageSprite.getTexture()->getSize().y);
//
//    }
  }

  m_gameWindow.display();
}


void GameChoosingState::updateAvailableGames(const std::string& combinedString)
{
  m_gamesMetaInfo.addMetaData(combinedString);
  m_updateMetaInfo = true;
}

  //const auto& plametas = m_gamesMetaInfo.getPlametaParsers();

  //auto& parser = plametas.find(key)->second;
  //auto gameNameLabel = sfg::Label::Create(std::get<std::string>(parser["overview:name"]->getVariant()));

} // namespace
