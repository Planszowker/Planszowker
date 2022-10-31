local Entity = {}

function Entity:new(id, texture, size, position)
    local entity = {}
    local metatable = {
        __index = self,
        __metatable = self
    }
    setmetatable(entity, metatable)

    entity._id = id or ""
    entity._texture = texture or ""
    entity._size = size or ""
    entity._position = position or ""

    return entity
end

---------------------
-- Setters/Getters --
---------------------
function Entity:SetTexture(texture)
    self._texture = texture

    ReplyModule:SetTexture(self._id, self._texture)
end

function Entity:GetTexture()
    return self._texture
end

function Entity:GetID()
    return self._id
end

function Entity:SetSize(size)
    self._size = size
end

function Entity:GetSize()
    return self._size
end

function Entity:SetPosition(position)
    self._position = position
end

function Entity:GetPosition()
    return self._position
end

return Entity