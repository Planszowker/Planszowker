#include "Logic.h"

namespace pla::common::games::server {

Logic::Logic(std::vector<size_t> &clientIds, const std::string& gameName)
  : m_clientIds(clientIds)
  , m_currentTurnClientId(clientIds[0])
  , m_finished(false)
  , m_gameName(gameName)
{
  m_luaVM.open_libraries(sol::lib::base,
                         sol::lib::package,
                         sol::lib::io,
                         sol::lib::table,
                         sol::lib::string);

  try {
    sol::protected_function_result result = m_luaVM.script_file("lua-scripts/games/" + m_gameName + "/" + m_gameName + "-states.lua");
    //sol::protected_function_result result2 = lua.script_file("lua-scripts/games/DiceRoller/DiceRoller.lua");
  } catch(sol::error& e) {
    std::cerr << "Exception has been raised! " << e.what() << "\n";
  }
}

bool Logic::_checkIfTurnAvailable(size_t clientId) const {
  return m_currentTurnClientId == clientId;
}

void Logic::handleGameLogic(size_t clientId, Request requestType,
                                      network::ServerPacketHandler &packetHandler)
{
  for (const auto& key: m_clientIds) {
    std::cout << "Available clientID: " << key << "\n";
  }

  sf::Packet replyToClient;
  Reply replyStruct {};

  // TODO: bug when previous client was deleted before game - BRD-15
  if (!_checkIfTurnAvailable(clientId)) {
    packetHandler.sendPacketToClient(clientId, replyToClient);
    return;
  }

  replyStruct.status = ReplyType::Success;

  std::cout << "DEBUG: Current turn was " << m_currentTurnClientId << "\n";
}


} // namespaces
