print('[LUA] Kingdomino init...')

--[[
  Create State Machine
--]]
StateMachine = Machine.create(
  {
    initial = 'InitState',
    events = {
      { name = 'ChooseDominoEvent', from = {'InitState', 'AttachedDominoState'}, to = 'ChoseDominoState'},
      { name = 'AttachDominoEvent', from = 'ChoseDominoState', to = 'AttachedDominoState' }
    }
  }
)

-- Overall number of tiles available in game
local numberOfTiles = 48

-- Map entities into separate table, it will be easier to use them
Entities = GameObjects.Entities

-- This variable holds grids created for each player
Grids = {}

-- We want to use server's RNG, thus we need to create one.
TileRng = Rng.new(1, numberOfTiles)

-- Update grid size according to connected players
local gridSize = 5
if #GetPlayers() == 2 then
  -- In case we have 2 players, grid size is increased from 5x5 to 7x7
  gridSize = 7
end

-- Iterate over all connected players and create a grid for each of them
for i = 1, #GetPlayers() do
  Grids[i] = Grid:New({x = gridSize, y = gridSize})
end

-- Create table with all tiles available throughout the game
Tiles = {}
for i = 1, numberOfTiles do
  Tiles[i] = Tile:New({x = 2, y = 1})
  Tiles[i]:SetRotable(true)
end

--============================================================================--
--                                 CALLBACKS                                  --
--============================================================================--

--[[
  Callback for ChooseDominoEvent

  It is called after event was fired.
--]]
StateMachine['onChooseDomino'] = function()
  print('ChooseDomino state!')



end

--[[
  Callback for AttachDominoEvent

  It is called after event was fired.
--]]
StateMachine['onAttachDomino'] = function()
  print('AttachDominoEvent has fired!')

end
