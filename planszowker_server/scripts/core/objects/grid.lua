--[[
  Grid object

  This object describes a grid (aka board) where tiles can be attached.
  This is a fixed size grid for rectangular tiles.

  Size of a grid cannot be changed at the moment after creation.

  @future Other grid tiles (HexGrid, OctoGrid, etc.)
]]

local Grid = {}

--[[
  Constructor

  @param[in] size Size table (with indexes 'x' and 'y')
--]]
function Grid:new(size)
  local grid = {}
  local metatable = {
    __index = self,
    __metatable = self
  }
  setmetatable(grid, metatable)

  grid._size.x = size.x or 0
  grid._size.y = size.y or 0

  return grid
end

function Grid:AttachTile(tile)
  -- @TODO
end



---------------------
-- Setters/Getters --
---------------------
function Grid:GetSize()
  return self._size
end

return Grid