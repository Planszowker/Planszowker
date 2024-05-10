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

  @param[in] size: Size table (with indexes 'x' and 'y')
--]]
function Grid:New(size)
  local grid = {}
  local metatable = {
    __index = self,
    __metatable = self
  }
  setmetatable(grid, metatable)

  grid._size = {}
  grid._size.x = size.x or 0
  grid._size.y = size.y or 0
  grid.TILE_OCCUPATION = { VACANT = 'o', OCCUPIED = 'x' }

  grid._grid = {}
  grid:_createGrid()

  return grid
end

--[[
  Attach tile at given position

  @param[in] tile: Tile to attach to grid
  @param[in] position: Position (X,Y) at which tile should be inserted
  @return True if tile has been inserted successfully, false otherwise
--]]
function Grid:AttachTile(tile, position)
  -- If tile is bigger than our grid, we need to return false immediately
  if tile:GetSize().width > self._size.x or tile:GetSize().height > self._size.y then
    return false
  end

  -- If position is negative, we need to check if shift is available
  -- First, check X
  local shift_grid = false
  if position.x < 0 then
    for x = self._size.x - math.abs(position.x), self._size.x - 1 do
      for y = 0, self._size.y - 1 do
        -- If tile in grid is already occupied, return false
        if self._grid[x][y] == self.TILE_OCCUPATION.OCCUPIED then
          return false
        end
      end
    end
    shift_grid = true
  end
  -- Then, check Y
  if position.y < 0 then
    for y = self._size.y - math.abs(position.y), self._size.y - 1 do
      for x = 0, self._size.x - 1 do
        -- If tile in grid is already occupied, return false
        if self._grid[x][y] == self.TILE_OCCUPATION.OCCUPIED then
          return false
        end
      end
    end
    shift_grid = true
  end

  -- Check if grid is not already occupied
  for tileX = 1, tile:GetSize().width do
    for tileY = 1, tile:GetSize().height do

      if position.x + tileX - 1 < 0 then goto omit_check end
      if position.y + tileY - 1 < 0 then goto omit_check end

      -- If tile in grid is already occupied, return false
      if self._grid[position.x + tileX - 1][position.y + tileY - 1] == self.TILE_OCCUPATION.OCCUPIED then
        return false
      end

      ::omit_check::
    end
  end

  if shift_grid then
    local shift = {x = 0, y = 0}
    if position.x < 0 then
      shift.x = math.abs(position.x)
      position.x = 0
    end
    if position.y < 0 then
      shift.y = math.abs(position.y)
      position.y = 0
    end

    self:_shift(shift)
  end

  -- If all check were successfully, we can insert tile to the grid
  for tileX = 1, tile:GetSize().width do
    for tileY = 1, tile:GetSize().height do
      self._grid[position.x + tileX - 1][position.y + tileY - 1] = self.TILE_OCCUPATION.OCCUPIED
    end
  end

  return true
end

function Grid:Print()
  print('=-=-=-=-=')
  -- Printing in console implies iterating over a row first
  for y = 0, self._size.y - 1 do
    for x = 0, self._size.x - 1 do
      io.write(self._grid[x][y])
    end
    io.write('\n')
  end
  io.write('\n')
end

---------------------
-- Setters/Getters --
---------------------
function Grid:GetSize()
  return self._size
end

---------------------
-- Private methods --
---------------------
function Grid:_createGrid()
  for x = 0, self._size.x - 1 do
    self._grid[x] = {}
    for y = 0, self._size.y - 1 do
      self._grid[x][y] = self.TILE_OCCUPATION.VACANT
    end
  end
end

-- Shift grid by given vector
function Grid:_shift(vec)

  -- We cannot copy by reference here, hence this weird assignment
  local oldGrid = {}
  for k, v in pairs(self._grid) do oldGrid[k] = v end

  self:_createGrid()

  for x = self._size.x - 1, vec.x, -1 do
    for y = self._size.y - 1, vec.y, -1 do
      self._grid[x][y] = oldGrid[x - vec.x][y - vec.y]
    end
  end
end

return Grid
