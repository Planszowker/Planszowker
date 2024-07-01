#pragma once

/* Generic */
#include "GraphicalView.h"
#include <Games/CommObjects.h>
#include <Games/GamesMetaInfo.h>

#include <NetworkHandler/ClientPacketHandler.h>

/* SFML */
#include <SFML/Network.hpp>

/* STD */
#include <memory>
#include <any>
#include <atomic>

namespace pla::games_client {

class GraphicalView;

/*!
 * @brief Controller is used to create threads for Graphical View and Network Packet Handler.
 * Packet Handler is used to gather all requests/responses from the Server. Controller is responsible for parsing them and
 * create appropriate response to the Server.
 */
class Controller
{
public:
  explicit Controller(sf::TcpSocket& serverSocket);

  void run();

  GraphicalView* getView()
  {
    return m_view.get();
  }

  network::ClientPacketHandler* getPacketHandler()
  {
    return &m_clientPacketHandler;
  }

  void sendRequest(games::PacketType type, const std::string& body = "");

private:
  void update();

  std::atomic_bool m_run {true}; ///< Flag used to sync threads.

  std::mutex m_mutex; ///< Mutex for shared resources.

  size_t m_clientID {0};

  network::ClientPacketHandler m_clientPacketHandler;

  std::shared_ptr<GraphicalView> m_view;

  games::GamesMetaInfo m_gamesMetaInfo;
};

} // namespaces
