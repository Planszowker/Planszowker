#include "Logic.h"

namespace pla::common::games::dice_roller {


DiceRollerLogic::DiceRollerLogic(std::vector<size_t> &clientIds)
  : m_clientIds(clientIds)
  , m_currentTurnId(clientIds[0])
  , m_rngGenerator(1,6)
  , m_dice({0, 0, 0})
  , m_rounds(1)
  , m_finished(false)
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

        auto it = m_dice.begin();
        for (const auto& die: diceReplyData.dice) {
          *(it++) = die;
        }

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

        auto it = m_dice.begin();
        for (const auto& die: diceReplyData.dice) {
          *(it++) = die;
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

        for (const auto& die : m_dice) {
          m_playerPoints[clientId] += die;
        }

        replyStruct.replyType = DiceRollerReplyType::PointsReply;

        DiceRollerPointsReplyData pointsReplyData {
          .clientId = clientId,
          .points = m_playerPoints[clientId],
          .currentRound = m_rounds
        };

        replyToClient.append(&replyStruct, sizeof(replyStruct));
        replyToClient.append(&pointsReplyData, sizeof(pointsReplyData));
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

      // Advance round
      if (m_rounds < MaxRounds) {
        ++m_rounds;
      } else {
        m_finished = true;
        replyToClient.clear();

        // TODO: Find better way to check it
        size_t winnerId = 0;
        bool draw = false;
        if (m_playerPoints[m_clientIds[0]] > m_playerPoints[m_clientIds[1]]) {
          winnerId = m_clientIds[0];
        } else if (m_playerPoints[m_clientIds[0]] < m_playerPoints[m_clientIds[1]]) {
          winnerId = m_clientIds[1];
        } else {
          draw = true;
        }

        replyStruct.replyType = DiceRollerReplyType::FinishReply;
        DiceRollerFinishReplyData finishReplyData {
          .winnerClientId = winnerId,
          .points = m_playerPoints[winnerId],
          .draw = draw
        };

        replyToClient.append(&replyStruct, sizeof(replyStruct));
        replyToClient.append(&finishReplyData, sizeof(finishReplyData));
        packetHandler.sendPacketToEveryClients(replyToClient);

        return;
      }
    }

    std::cout << "DEBUG: Current turn was " << m_currentTurnId << "\n";
    m_currentTurnId = *keyIt;

    m_state = State::Entry;

    std::cout << "New client ID turn: " << m_currentTurnId << "\n";
  }
}


} // namespaces
