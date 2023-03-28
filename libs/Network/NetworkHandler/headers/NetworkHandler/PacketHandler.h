#pragma once

#include <atomic>
#include <mutex>
#include <thread>

namespace pla::network {

/*!
 *  @brief Class for handling network communication.
 *  It receives packets from end points and stores them.
 */
class PacketHandler {
public:
  explicit PacketHandler(std::atomic_bool& run) : m_run(run) { }

  virtual void runInBackground() = 0;

  virtual void stop() = 0;

protected:
  virtual void _backgroundTask() = 0;

  // Multithreading variables
  std::thread m_backgroundThread;
  std::mutex m_tcpSocketsMutex;
  std::atomic_bool& m_run;
};

}