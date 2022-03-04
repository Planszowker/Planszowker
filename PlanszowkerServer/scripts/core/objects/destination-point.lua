local DestinationPoint = {}

function DestinationPoint:new(id, position)
    local destinationPoint = {}
    local metatable = {
        __index = self,
        __metatable = self
    }
    setmetatable(destinationPoint, metatable)

    destinationPoint._id = id or ""
    destinationPoint._position = position or ""

    return destinationPoint
end

---------------------
-- Setters/Getters --
---------------------
function DestinationPoint:GetID()
    return self._id
end

function DestinationPoint:SetPosition(position)
    self._position = position
end

function DestinationPoint:GetPosition()
    return self._position
end

return DestinationPoint