Entity = require('entity')

-- Create entity with default values
local entity = Entity:new()

-- Create entity with given values
local entityWithValues = Entity:new("SomeEntity", "Texture.png", "0.1", "0.1x0.1")

assert(entity:GetID() == "", "ID is wrong!")
assert(entity:GetPosition() == "", "Position is wrong!")
assert(entity:GetSize() == "", "Size is wrong!")
assert(entity:GetTexture() == "", "Texture is wrong!")

assert(entityWithValues:GetID() == "SomeEntity", "ID is wrong!")
assert(entityWithValues:GetPosition() == "0.1x0.1", "Position is wrong!")
assert(entityWithValues:GetSize() == "0.1", "Size is wrong!")
assert(entityWithValues:GetTexture() == "Texture.png", "Texture is wrong!")

print("[LUA TEST] Entity test has passed!")