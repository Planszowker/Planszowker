#pragma once

#include <string>
#include <cstdint>

#include <SFML/Network.hpp>

namespace pla::games {

namespace json_entries {
auto constexpr CREATOR_ID = "CreatorID";            ///< Number: Creator's ID of specific lobby.
auto constexpr CLIENT_ID = "ClientID";              ///< Number: Specific Client's ID.
auto constexpr CLIENT_IDS = "ClientIDs";            ///< Array of numbers: List of all Clients connected to the specific lobby.
auto constexpr LOBBY_NAME = "LobbyName";            ///< String: Name of specific lobby.
auto constexpr GAME_KEY = "GameKey";                ///< String: Game key.
auto constexpr MAX_PLAYERS = "MaxPlayers";          ///< Number: Maximal number of players game can handle.
auto constexpr MIN_PLAYERS = "MinPlayers";          ///< Number: Minimal number of players to start the game.
auto constexpr CURRENT_PLAYERS = "CurrentPlayers";  ///< Number of players currently connected to given lobby.
auto constexpr VALID = "Valid";                     ///< Boolean: True if JSON is valid.
auto constexpr LOBBIES = "Lobbies";                 ///< Array of objects: List of lobbies available for given game.
auto constexpr LOBBY_HEARTBEAT_TYPE = "Type";       ///< String: Type of heartbeat sent by Client.
}

enum class PacketType : uint8_t
{
  Heartbeat,           ///< Used for pinging clients.
  GameSpecificData,    ///< Used for game specific data.
  ID,                  ///< Used to get client's ID associated in server.
  DownloadAssets,      ///< Used to get game's assets - only textures for now.
  StartTransaction,    ///< Used to start asset transaction.
  EndTransaction,      ///< Used to end asset transaction.
  ListAvailableGames,  ///< Used to list all games the server is able to handle.
  CreateLobby,         ///< Used to create a lobby with given name.
  GetLobbyDetails,     ///< Used to retrieve details about a specific lobby.
  ListOpenLobbies,     ///< Used to list all open lobbies for a specific game.
  JoinLobby,           ///< Used to connect a Client to a specific lobby (not self created).
  LobbyHeartbeat,      ///< Used for keeping lobby alive. If we stop sending this packets for defined amount of time, lobby for current Client is destroyed.
  DisconnectClient,    ///< Used to inform Client that he was disconnected from lobby, so GUI could be updated.
  StartGame,           ///< Used to start a specific game.
  AssetTransaction,    ///< Used to transmit a supported asset to client.
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
