#include "ClientInfo.h"

namespace pla::client_info {

ClientInfo::ClientInfo(const sf::IpAddress& ipAddress, const unsigned short& port, const size_t id)
  : m_ip(ipAddress), m_port(port), m_id(id)
{
}

} // namespaces
