local ActionButton = {}

function ActionButton:new(id, visible)
    local actionButton = {}
    local metatable = {
        __index = self,
        __metatable = self
    }
    setmetatable(actionButton, metatable)

    actionButton._id = id or ""
    actionButton._visible = visible or false

    return actionButton
end

---------------------
-- Setters/Getters --
---------------------
function ActionButton:GetID()
    return self._id
end

function ActionButton:SetVisibility(visible)
    self._visible = visible
end

function ActionButton:GetVisibility()
    return self._visible
end

return ActionButton