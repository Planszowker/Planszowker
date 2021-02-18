#include "ClientInfo.h"

namespace pla::common::client_info {

ClientInfo::ClientInfo(const sf::IpAddress& ipAddr, const unsigned short& port)
  : m_Ip(ipAddr), m_Port(port)
{

}


const sf::IpAddress& ClientInfo::getIpAddress()
{
  return m_Ip;
}


const unsigned short& ClientInfo::getPort()
{
  return m_Port;
}

} // namespaces
