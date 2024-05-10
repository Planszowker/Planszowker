script_path = string.gsub(arg[0], '^(.+/)[^/]+$', '%1')

package.path = script_path .. '../objects/?.lua;' .. package.path
package.path = script_path .. '../luaunit/?.lua;' .. package.path
package.path = script_path .. '../debug/?.lua;' .. package.path

print(package.path)

lu = require('luaunit')
inspect = require('inspect')

Grid = require('grid')
Tile = require('tile')

print(inspect(Grid))

grid = Grid:New({x = 5, y = 5})
grid:Print()
-- ooooo
-- ooooo
-- ooooo
-- ooooo
-- ooooo

tile = Tile:New({width = 3, height = 1})
print(grid:AttachTile(tile, {x = 0, y = 0}))
grid:Print()
-- xxxoo
-- ooooo
-- ooooo
-- ooooo
-- ooooo

tile = Tile:New({width = 3, height = 1})
print(grid:AttachTile(tile, {x = 0, y = 2}))
grid:Print()
-- xxxoo
-- ooooo
-- xxxoo
-- ooooo
-- ooooo

tile = Tile:New({width = 2, height = 1})
print(grid:AttachTile(tile, {x = -2, y = 0}))
grid:Print()
-- xxxxx
-- ooooo
-- ooxxx
-- ooooo
-- ooooo

-- This should not be allowed
tile = Tile:New({width = 3, height = 2})
print(grid:AttachTile(tile, {x = 2, y = -1}))
grid:Print()


tile = Tile:New({width = 3, height = 2})
print(grid:AttachTile(tile, {x = 2, y = -2}))
grid:Print()
-- ooxxx
-- ooxxx
-- xxxxx
-- ooooo
-- ooxxx


tile = Tile:New({width = 1, height = 2})
print(grid:AttachTile(tile, {x = 0, y = 0}))
grid:Print()
-- xoxxx
-- xoxxx
-- xxxxx
-- ooooo
-- ooxxx


tile = Tile:New({width = 1, height = 2})
print(grid:AttachTile(tile, {x = 1, y = 0}))
grid:Print()
-- xxxxx
-- xxxxx
-- xxxxx
-- ooooo
-- ooxxx
