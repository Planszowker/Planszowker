#pragma once

#include <string>
#include <vector>
#include <utility>

namespace pla::supervisor {

class Lobby
{
public:
  Lobby() = delete;
  Lobby(size_t creatorClientId, std::string lobbyName, std::string gameKey);

  [[maybe_unused]] [[nodiscard]]
  size_t getCreatorClientId() const { return m_creatorClientId; }
private:
  void _extractGameMetadata();
  size_t m_creatorClientId;
  std::string m_lobbyName;
  std::string m_gameKey;

  std::vector<size_t> m_clients;
};

}