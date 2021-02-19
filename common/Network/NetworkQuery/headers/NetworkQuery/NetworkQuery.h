#pragma once

#include "ClientInfo/ClientInfo.h"

#include <SFML/Network.hpp>
#include <unordered_map>

namespace pla::common::network {

/*!
 *  \brief Class for querying clients to check their
 *
 * [TODO]: Extend this class in future.
 *  This class uses SFML Networking module for communication.
 */
class NetworkQuery
{
public:
  static bool queryClient(sf::TcpSocket& socket, std::map<size_t, common::client_info::ClientInfo>);
};

} // namespaces