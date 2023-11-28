--[[
  Tile object

  This object describes a rectangular tile. A tile has its size defined.
  Rectangular tiles shall be used alongside with grid.

  @see Grid

  @future Other tile types
]]

local Tile = {}

--[[
  Constructor

  @param[in] size Size table (with indexes 'x' and 'y')
--]]
function Tile:new(size)
  local tile = {}
  local metatable = {
    __index = self,
    __metatable = self
  }
  setmetatable(tile, metatable)

  tile._size.x = size.x
  tile._size.y = size.y

  return tile
end

---------------------
-- Setters/Getters --
---------------------
function Tile:GetSize()
  return self._size
end

return Tile