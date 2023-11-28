DestinationPoint = require('destination-point')

-- Create destination point with default values
local destinationPoint = DestinationPoint:new()

-- Create destination point with given values
local destinationPointWithValues = DestinationPoint:new("SomeEntity", "0.1x0.1")

assert(destinationPoint:GetID() == "", "ID is wrong!")
assert(destinationPoint:GetPosition() == "", "Position is wrong!")

assert(destinationPointWithValues:GetID() == "SomeEntity", "ID is wrong!")
assert(destinationPointWithValues:GetPosition() == "0.1x0.1", "Position is wrong!")

print("[LUA TEST] DestinationPoint test has passed!")