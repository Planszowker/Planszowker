ActionButton = require('action-button')

-- Create action button point with default values
local actionButton = DestinationPoint:new()

-- Create destination point with given values
local actionButtonWithValues = DestinationPoint:new("SomeActionButton", true)

assert(actionButton:GetID() == "", "ID is wrong!")
assert(actionButton:GetVisibility() == false, "Visibility is wrong!")

assert(actionButtonWithValues:GetID() == "SomeActionButton", "ID is wrong!")
assert(actionButtonWithValues:GetVisibility() == true, "Visibility is wrong!")

print("[LUA TEST] ActionButton test has passed!")