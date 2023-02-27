#pragma once

#include <string>
#include <cstdint>

#include <SFML/Network.hpp>

namespace pla::games {

enum class PacketType : uint8_t
{
  Heartbeat, ///< Used for pinging clients.
  GameSpecificData, ///< Used for game specific data.
  ID, ///< Used to get client's ID associated in server.
  DownloadAssets, ///< Used to get game's assets - only textures for now.
  StartTransaction, ///< Used to start asset transaction.
  EndTransaction, ///< Used to end asset transaction.
  ListAvailableGames, ///< Used to list all games the server is able to handle.
  CreateLobby, ///< Used to create a lobby with given name.
  GetLobbyDetails, ///< Used to retrieve details about a specific lobby.
  ListOpenLobbies, ///< Used to list all open lobbies for a specific game.
  JoinLobby, ///< Used to connect a Client to a specific lobby (not self created).
  LobbyHeartbeat, ///< Used for keeping lobby alive. If we stop sending this packets for defined amount of time, lobby for current Client is destroyed.
  ClientDisconnected, ///< Used to inform Client that he was disconnected from lobby, so GUI could be updated.
  AssetTransaction, ///< Used to transmit a supported asset to client.
};


enum class ReplyType : uint8_t
{
  Success = 0, ///< Successful reply, queried action has been done successfully.
  Invalid ///< Invalid reply, received when some action cannot be performed.
};


/*!
 * @brief Structure that is sent to server.
 */
struct Request
{
  PacketType type = PacketType::Heartbeat;
  std::string body;
};


/*!
 * @brief Structure that is received from server.
 */
struct Reply
{
  PacketType type = PacketType::GameSpecificData;
  ReplyType status = ReplyType::Invalid;
  std::string body;
};


/*!
 * @brief Structure of supported assets extensions
 */
enum class SupportedAssetExtension : uint8_t
{

};

} // namespaces

sf::Packet& operator << (sf::Packet& packet, const pla::games::Request& request);
sf::Packet& operator >> (sf::Packet& packet, pla::games::Request& request);
sf::Packet& operator << (sf::Packet& packet, const pla::games::Reply& reply);
sf::Packet& operator >> (sf::Packet& packet, pla::games::Reply& reply);
