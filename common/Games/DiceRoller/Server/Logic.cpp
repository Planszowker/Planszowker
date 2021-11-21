#include "Logic.h"

namespace pla::common::games::dice_roller {


DiceRollerLogic::DiceRollerLogic(std::vector<size_t> &clientIds)
  : m_clientIds(clientIds)
  , m_currentTurnId(clientIds[0])
  , m_rngGenerator(1,6)
{
}

bool DiceRollerLogic::_checkIfTurnAvailable(size_t clientId) {
  return m_currentTurnId == clientId;
}

void DiceRollerLogic::handleGameLogic(size_t clientId, DiceRollerRequestType requestType,
                                      network::ServerPacketHandler &packetHandler)
{
  for (const auto& key: m_clientIds) {
    std::cout << "Available clientID: " << key << "\n";
  }

  sf::Packet replyToClient;
  DiceRollerReply replyStruct {};

  // TODO: bug when previous client was deleted before game - BRD-15
  if (!_checkIfTurnAvailable(clientId)) {
    packetHandler.sendPacketToClient(clientId, replyToClient);
    return;
  }

  replyStruct.status = ReplyType::Success;
  replyStruct.packetType = PacketType::GameSpecificData;

  switch (requestType) {
    case DiceRollerRequestType::RollDice:
      if (m_state == State::Entry) {
        // Roll dice
        std::cout << "Roll dice for client " << clientId << "\n";

        DiceRollerDiceReplyData diceReplyData { .clientId = m_currentTurnId, .dice = {
                m_rngGenerator.generateRandomNumber(),
                m_rngGenerator.generateRandomNumber(),
                m_rngGenerator.generateRandomNumber()
        } };
        replyStruct.replyType = DiceRollerReplyType::DiceReply;

        replyToClient.append(&replyStruct, sizeof(replyStruct));
        replyToClient.append(&diceReplyData, sizeof(replyToClient));
        packetHandler.sendPacketToEveryClients(replyToClient);

        m_state = State::Rolled;
      }
      break;

    case DiceRollerRequestType::Reroll:
      if (m_state == State::Rolled) {
        // Reroll dice
        std::cout << "Re-roll dice for client " << clientId << "\n";

        DiceRollerDiceReplyData diceReplyData { .clientId = m_currentTurnId, .dice = {
                m_rngGenerator.generateRandomNumber(),
                m_rngGenerator.generateRandomNumber(),
                m_rngGenerator.generateRandomNumber()
        } };

        for (int i = 0; i < sizeof(diceReplyData.dice) / sizeof(uint8_t); ++i) {
          m_playerPoints[clientId] += diceReplyData.dice[i];
        }

        replyStruct.replyType = DiceRollerReplyType::DiceReply;

        replyToClient.append(&replyStruct, sizeof(replyStruct));
        replyToClient.append(&diceReplyData, sizeof(replyToClient));
        packetHandler.sendPacketToEveryClients(replyToClient);

        m_state = State::Rerolled;
      }
      break;

    case DiceRollerRequestType::Confirm:
      if (m_state == State::Rolled || m_state == State::Rerolled) {
        // Confirm rolls
        std::cout << "Confirm rolls for client " << clientId << "\n";

        replyStruct.replyType = DiceRollerReplyType::ReplyWithoutData;

        replyToClient.append(&replyStruct, sizeof(replyStruct));
        packetHandler.sendPacketToEveryClients(replyToClient);

        m_state = State::Confirmed;
      }
      break;

    default:
      break;
  }

  // End of turn for current client ID
  if (m_state == State::Confirmed) {
    std::cout << "Confirmed client " << clientId << "\n";

    auto keyIt = std::find(m_clientIds.begin(), m_clientIds.end(), clientId);
    if (keyIt != m_clientIds.end()) {
      std::cout << "DEBUG: std::next\n";
      keyIt = std::next(keyIt, 1);
    }

    if (keyIt == m_clientIds.end()) {
      std::cout << "DEBUG: m_clientIds begin\n";
      keyIt = m_clientIds.begin();
    }

    std::cout << "DEBUG: Current turn was " << m_currentTurnId << "\n";
    m_currentTurnId = *keyIt;
    m_state = State::Entry;

    std::cout << "New client ID turn: " << m_currentTurnId << "\n";
  }
}


} // namespaces
