--[[
  Tile object

  This object describes a rectangular tile. A tile has its size defined.
  Rectangular tiles shall be used alongside with grid.

  @see Grid

  @note Tile 3x2 means 3 columns (across X-axis) and 2 rows (across Y-axis) a.k.a. tile is column-wise.

  @future Other tile types
]]

local Tile = {}

--[[
  Constructor

  @param[in] size Size table (with indexes 'x' and 'y')
--]]
function Tile:New(size)
  local tile = {}
  local metatable = {
    __index = self,
    __metatable = self
  }
  setmetatable(tile, metatable)

  tile._size = {}
  tile._size.width = size.width
  tile._size.height = size.height
  tile._rotation = 0
  tile._rotable = false

  return tile
end

---------------------
-- Setters/Getters --
---------------------
function Tile:GetSize()
  return self._size
end

function Tile:SetRotation(rotation)
  -- Accept only values divisible by 90 deg
  if rotation % 90 == 0 and self._rotable then
    self._rotation = rotation
  end
end

function Tile:GetRotation()
  return self._rotation
end

function Tile:SetRotable(rotable)
  self._rotable = rotable
end

function Tile:IsRotable()
  return self._rotable
end

return Tile
