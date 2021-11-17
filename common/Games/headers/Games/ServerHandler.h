#pragma once

/* Generic */
#include "NetworkHandler/ServerPacketHandler.h"

/* SFML */
#include <SFML/Network.hpp>

/* STD */
#include <vector>
#include <memory>
#include <atomic>

namespace pla::common::games {

class ServerHandler
{
public:
  explicit ServerHandler(network::ServerPacketHandler& packetHandler)
    : m_packetHandler(packetHandler)
    , m_run(true)
  {
  }

  virtual ~ServerHandler()
  {
    m_packetHandler.stop();
  }

  virtual void run()
  {
    m_packetHandler.runInBackground();

    while(m_run)
    {
      if (!_internalHandling()) {
        break;
      }
    }
  }

protected:
  /*
   * Method that every game need to override. Game has to handle packets from ServerPacketHandler, process them,
   * and update game logic as needed.
   */
  virtual bool _internalHandling() = 0;

  bool m_run;
  network::ServerPacketHandler& m_packetHandler;
};

} // namespaces
