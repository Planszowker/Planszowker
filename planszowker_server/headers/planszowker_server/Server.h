#pragma once

#include <thread>

namespace pla::server {

/*!
 *  \brief Server class. It is a main class used to create a server.
 *
 *  Exceptions will be thrown by this class, so one must catch them outside the function
 */
class Server
{
public:

  /*!
   *  \brief Method to run server in a loop. It DOES NOT contain an infinite loop.
   *
   *  What is does:
   *   * It creates NetworkHandler object in another thread to handle communication
   *   * [TODO]
   */
  void run() { }
private:
  //std::thread
};

} // namespaces
