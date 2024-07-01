#include "Controller.h"

#include "Logger/Logger.h"
#include "ErrorHandler/ErrorLogger.h"
#include "Games/CommObjects.h"
#include "NetworkHandler/ClientPacketHandler.h"
#include "Games/GamesMetaInfo.h"

#include <thread>
#include <chrono>
#include <unordered_map>
#include <sstream>

#include <nlohmann/json.hpp>

#include <easylogging++.h>

using namespace pla;
using namespace pla::err_handler;
using namespace pla::logger;
using namespace pla::games;

namespace pla::games_client {

Controller::Controller(sf::TcpSocket& serverSocket)
  : m_clientPacketHandler(m_run, serverSocket)
  , m_view(std::make_shared<GraphicalView>(*this, m_run, sf::Vector2i(1200U, 960), "Planszowker Client"))
{
  m_view->init();
}


void Controller::run() {
  m_clientPacketHandler.runInBackground();

  m_view->run();
}


void Controller::update()
{

}


void Controller::sendRequest(games::PacketType type, const std::string& body)
{
  sf::Packet requestPacket;
  games::Request request {
    .type = type,
    .body = body
  };

  requestPacket << request;

  m_clientPacketHandler.sendPacket(requestPacket);
}

} // namespace
