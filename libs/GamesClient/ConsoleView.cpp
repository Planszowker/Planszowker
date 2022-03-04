#include "ConsoleView.h"

#include "Games/Objects.h"

#include <iostream>
#include <chrono>
#include <string>
#include <atomic>

#include <SFML/Graphics.hpp>

#include <AssetsManager/AssetsReceiver.h>

using namespace pla::games;

namespace pla::common::games::dice_roller {

void DiceRollerConsoleView::update(const std::any& object) {
  auto receivedObject = std::any_cast<std::string>(object);

  std::cout << receivedObject << std::endl;

  // TODO: Remove below printout...
  //std::cout << "Received additional info: " << receivedObject.additionalInfo << "\n";
  //std::cout << "Reply: " << static_cast<size_t>(receivedObject.reply) << "\n";
}


void DiceRollerConsoleView::init() {
  // No mutex is needed here, since it is invoked before any multithreading

  // Some basic info about a game
  std::cout << "DiceRoller v 0.1 (LUA)\n";
  std::cout << "========================================\n";
  std::cout << "Choose an action. You can reroll up to 1 time. Points are summed. Player with a greater score wins.\n";
  std::cout << "========================================\n";
}


void DiceRollerConsoleView::notifyController(std::function<void(std::any&)> callback) {
  // TODO: It is just for testing
  Request requestToSend;

  requestToSend.type = PacketType::GameSpecificData;
  switch(m_inputType){
    case 1:
      requestToSend.body = "{"
                           "\"ActionRequests\": [{"
                           "\"Action\": \"ButtonPress\","
                           "\"Info\": \"Roll\""
                           "}]"
                           "}";
      break;
    case 2:
      requestToSend.body = "{"
                           "\"ActionRequests\": [{"
                           "\"Action\": \"ButtonPress\","
                           "\"Info\": \"Reroll\""
                           "}]"
                           "}";
      break;
    case 3:
      requestToSend.body = "{"
                           "\"ActionRequests\": [{"
                           "\"Action\": \"ButtonPress\","
                           "\"Info\": \"Confirm\""
                           "}]"
                           "}";
      break;
    case 4:
      requestToSend.type = PacketType::DownloadAssets;
      break;
    case 5:
      requestToSend.type = PacketType::ListAvailableGames;
      break;
    default:
      break;
  }

  auto request = std::make_any<Request>(requestToSend);

  //std::cout << "Sending request type of value: " << static_cast<int>(std::any_cast<DiceRollerRequest>(requestToSend).type) << "\n";
  callback(request);
}


void DiceRollerConsoleView::runLoop(Controller* controller, std::atomic_bool& runLoop)
{
  while (runLoop)
  {
    // Wait for input
    std::cout << "Packet to send:\n"
                 "(1) Roll dice\n"
                 "(2) Re-roll dice\n"
                 "(3) Confirm\n"
                 "(4) Get assets\n"
                 "(5) List all games\n"
                 "Your choice: ";
    std::cin >> m_inputType;

    if (m_inputType < 1 || m_inputType > 5) {
      std::cout << "Wrong packet type!\n";
      continue;
    }

    std::function<void(std::any&)> callback = std::bind(&Controller::viewCallback, controller, std::placeholders::_1);
    notifyController(callback);
  }
}


void DiceRollerConsoleView::showAssets()
{
  size_t ctr {0};

  sf::RenderWindow window(sf::VideoMode(640, 640), "Assets previewer");
  window.setFramerateLimit(60.f);

  sf::Clock clk;

  while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        default:
          break;
      }
    }

    auto assets = assets::AssetsReceiver::getAssetNames();

    sf::Time elapsed = clk.getElapsedTime();
    if (!assets.empty() && elapsed.asSeconds() >= 1.f) {
      clk.restart();

      ++ctr;
      if (ctr >= assets.size()) {
        ctr = 0;
      }

      //std::cout << "Choosing " << assets[ctr] << " asset...\n";

      auto texture = assets::AssetsReceiver::getTexture(assets[ctr]);
      //std::cout << "Texture info " << texture->getSize().x << " x " << texture->getSize().y << " asset...\n";

      sf::Sprite sprite;
      sprite.setTexture(*texture);

      auto ziu = texture->getSize();
      sf::Vector2f factor (640.f / float(ziu.x), 640.f / float(ziu.y));

      sprite.setScale(factor);

      window.clear(sf::Color::Magenta);
      window.draw(sprite);
    }

    window.display();
  }

  std::cout << "Exiting Thread...\n";
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

} // namespaces
