# Reply JSONs

1. [Overall information](#overall-information)
2. [Get Lobby Details](#get-lobby-details)
3. [List Open Lobbies](#list-open-lobbies)
4. [Join Lobby](#join-lobby)
5. [Create Lobby](#create-lobby)
6. [Common fields](#common-fields)

### Overall information
Below are listed fields available in reply JSONs (sent **from Server to Client**).


### Get Lobby Details

* CreatorID (number) - Creator of specific lobby,
* ClientIDs (array of numbers) - List of all Clients connected to the specific lobby,
* LobbyName (string) - Name of specific lobby,
* GameKey (string) - Game key,
* MaxPlayers (number) - Maximal number of players game can handle,
* MinPlayers (number) - Minimal number of players to start the game,
* CurrentPlayers (number) - Number of players currently connected to given lobby,


### List Open Lobbies

* GameKey (string) - Game key,
* Lobbies (array of objects):
* CreatorID (number) - Creator of given lobby,
* CurrentPlayers (number) - Number of players currently connected to given lobby,
* LobbyName (string) - Name of given lobby,
* MaxPlayers (number) - Maximal number of players game can handle,
* MinPlayers (number) - Minimal number of players to start the game,


### Join Lobby

### Create Lobby

### Common fields
Below fields are available regardless of chosen reply type:
* Valid (boolean) - True if reply is valid.