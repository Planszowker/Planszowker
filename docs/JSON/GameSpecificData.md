# Game Specific Data JSONs

1. [Reply JSON](#reply-json)
   1. [Fields](#fields)
      1. [Actions](#actions)
         1. [SetTexture](#settexture)
         2. [SetVisibility](#setvisibility)
      2. [Events](#events)
      3. [GameFinished](#gamefinished)
      4. [PlayersInfo](#playersinfo)


## Reply JSON
Describes how game displaying should change, players' points, events and other game related stuff.

### Fields

#### Actions
Describe actions to be performed on games' objects.
##### SetTexture
This action sets texture on given entity.
###### Fields
* Action (string) - "SetTexture",
* Entity (string) - Entity's unique name,
* Texture (string) - New entity's texture (base directory is `<GameName>/Assets/`).
##### SetVisibility
This action sets visibility on given entity. Can be used to set visibility on buttons in Action Bar or Entity inside game area.
###### Fields
* Action (string) - "SetVisibility",
* ObjectID (string) - Object's unique ID,
* Visibility (boolean) - Updated visibility.

#### Events
Describe events that are logged in player's window.
TODO

#### GameFinished
Describes whether game has finished or not.
###### Fields
* GameFinished (boolean) - whether game has finished or not.

#### PlayersInfo
Describes information about players.
###### Fields
* ID (number) - Player's ID,
* Points (number) - Player's points (some games might not use that points system)