#pragma once

#include <atomic>
#include <mutex>
#include <thread>

namespace pla::common::network {

/*!
 *  @brief Class for handling network communication.
 *  It receives packets from end points and stores them.
 */
class PacketHandler {
public:
  PacketHandler() : m_run(false) { }

  virtual void runInBackground() = 0;

  virtual void stop() = 0;

protected:
  virtual void _backgroundTask(std::mutex&) = 0;

  // Multithreading variables
  std::thread m_backgroundThread;
  std::mutex m_tcpSocketsMutex;
  std::atomic_bool m_run;
};

}