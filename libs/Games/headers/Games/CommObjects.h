#pragma once

#include <string>
#include <cstdint>

#include <SFML/Network.hpp>

namespace pla::games {

namespace json_entries {
///////////////////////////////
// [SECTION] Generic entries //
///////////////////////////////
auto constexpr CREATOR_ID = "CreatorID";            ///< Number: Creator's ID of specific lobby.
auto constexpr CLIENT_ID = "ClientID";              ///< Number: Specific Client's ID.
auto constexpr CLIENT_IDS = "ClientIDs";            ///< Array of numbers: List of all Clients connected to the specific lobby.
auto constexpr LOBBY_NAME = "LobbyName";            ///< String: Name of specific lobby.
auto constexpr GAME_KEY = "GameKey";                ///< String: Game key.
auto constexpr MAX_PLAYERS = "MaxPlayers";          ///< Number: Maximal number of players game can handle.
auto constexpr MIN_PLAYERS = "MinPlayers";          ///< Number: Minimal number of players to start the game.
auto constexpr CURRENT_PLAYERS = "CurrentPlayers";  ///< Number of players currently connected to given lobby.
auto constexpr VALID = "Valid";                     ///< Boolean: True if response is valid.
auto constexpr LOBBIES = "Lobbies";                 ///< Array of objects: List of lobbies available for given game.
auto constexpr LOBBY_HEARTBEAT_TYPE = "Type";       ///< String: Type of heartbeat sent by Client.

// Assets transmitting
auto constexpr ASSET_NAME = "AssetName";            ///< String: Asset's name.
auto constexpr ASSET_TYPE = "AssetType";            ///< String: Type of asset ("Image", "BoardDescription").
auto constexpr ASSET_B64_DATA = "AssetData";        ///< String: Base64 asset string.

/////////////////////////////////////
// [SECTION] Game Specific Entries //
/////////////////////////////////////
auto constexpr ACTIONS = "Actions";                 ///< Array of objects: List of actions requests.
auto constexpr ACTION = "Action";                   ///< String: Action to be performed.
auto constexpr TURN_CLIENT_ID = "TurnClientID";     ///< String: Client ID with current turn.
auto constexpr GAME_FINISHED = "GameFinished";      ///< Boolean: True if game has finished.
auto constexpr PLAYERS_INFO = "PlayersInfo";        ///< Array of objects: List of players' info.
auto constexpr PLAYER_INFO_ID = "ID";               ///< String: Player's ID.
auto constexpr PLAYER_INFO_POINTS = "Points";       ///< Number: Player's points.
auto constexpr EVENTS = "Events";                   ///< Array of objects: Events that should be displayed in EventLog.
auto constexpr EVENTS_EVENT_STRING = "EventString"; ///< String: Event string.


//////////////////////////////////////
// [SECTION] Game Specific Requests //
//////////////////////////////////////
/*!
 * @brief Object Pressed action request
 * This request is used whenever a clickable object (entity, button, etc.)
 * is pressed.
 *
 * Request fields:
 *  - ID - unique object ID
 *
 * @{
 */
auto constexpr ACTION_OBJECT_PRESSED = "ObjectPressed";   ///< Action name
auto constexpr ACTION_OBJECT_PRESSED_OBJECT_ID = "ID";           ///< String: Object's ID that has been pressed.
/*! @} */


/////////////////////////////////////
// [SECTION] Game Specific Replies //
/////////////////////////////////////
/*!
 * @brief Set Texture reply
 * This reply is received when client should update entity's
 * texture.
 *
 * Reply fields:
 *  - Entity - entity's ID
 *  - Texture - new texture name
 *
 * @{
 */
auto constexpr ACTION_SET_TEXTURE = "SetTexture";       ///< Action name
auto constexpr ACTION_SET_TEXTURE_ENTITY_ID = "ID";     ///< String: Entity's ID that should have updated texture.
auto constexpr ACTION_SET_TEXTURE_TEXTURE = "Texture";  ///< String: Texture name (filename).
/*! @} */

/*!
 * @brief Set Object's Visibility reply
 * This reply is received when client should update object's
 * visibility. Non-visible objects are not rendered in scene.
 *
 * Reply fields:
 *  - ID - object's ID
 *  - Visibility - whether object should be visible or not
 *
 * @{
 */
auto constexpr ACTION_SET_VISIBILITY = "SetVisibility";           ///< Action name
auto constexpr ACTION_SET_VISIBILITY_OBJECT_ID = "ID";            ///< String: Object's ID that should have visibility updated.
auto constexpr ACTION_SET_VISIBILITY_VISIBILITY = "Visibility";   ///< Boolean: Wheter object should be visible or not.
/*! @} */
}

enum class PacketType : uint8_t
{
  Invalid,                     ///< Invalid packet.

  // General purpose
  Heartbeat,                   ///< Used for pinging clients.
  ID,                          ///< Used to get client's ID associated in server.

  // Transfer specific
  DownloadAssets,              ///< Used to indicate that Client wants to get game's assets.

  // Lobby specific
  ListAvailableGames,          ///< Used to list all games the server is able to handle.
  CreateLobby,                 ///< Used to create a lobby with given name.
  GetLobbyDetails,             ///< Used to retrieve details about a specific lobby.
  ListOpenLobbies,             ///< Used to list all open lobbies for a specific game.
  JoinLobby,                   ///< Used to connect a Client to a specific lobby (not self created).
  LobbyHeartbeat,              ///< Used for keeping lobby alive. If we stop sending this packets for defined amount of time, lobby for current Client is destroyed.
  DisconnectClient,            ///< Used to inform Client that he was disconnected from lobby, so GUI could be updated.
  StartGame,                   ///< Used to start a specific game.

  // Game specific
  GameSpecificData,            ///< Used for game specific data.
  IsTurnAvailable              ///< Used to check if it is a player's turn.
};


/*!
 * @brief Structure that is sent to server.
 */
struct Request
{
  PacketType type = PacketType::Invalid;
  std::string body;
};


/*!
 * @brief Structure that is received from server.
 */
struct Reply
{
  PacketType type = PacketType::Invalid;
  std::string body;
};


/*!
 * @brief Structure of supported assets extensions
 */
enum class SupportedAssetExtension
{
  // @TODO
};

} // namespaces

sf::Packet& operator << (sf::Packet& packet, const pla::games::Request& request);
sf::Packet& operator >> (sf::Packet& packet, pla::games::Request& request);
sf::Packet& operator << (sf::Packet& packet, const pla::games::Reply& reply);
sf::Packet& operator >> (sf::Packet& packet, pla::games::Reply& reply);
