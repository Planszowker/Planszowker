local GameObjects = {}

-- Parse BoardDescription into LUA table
local boardDescription = Json.decode(JsonString)
boardDescription['Entities'] = boardDescription['Entities'] or {}
boardDescription['DestinationPoints'] = boardDescription['DestinationPoints'] or {}

-- Parse Entities
GameObjects['Entities'] = {}
for _, v in ipairs(boardDescription['Entities']) do
    print("Adding entity with ID "..v['ID'])
    GameObjects['Entities'][v['ID']] = Entity:new(
        v['ID'],
        v['Texture'],
        v['Size'],
        v['Position']
    )
end

-- Parse DestinationPoints
GameObjects['DestinationPoints'] = {}
for _, v in ipairs(boardDescription['DestinationPoints']) do
    GameObjects['DestinationPoints'][v['ID']] = DestinationPoint:new(
            v['ID'],
            v['Position']
    )
end

return GameObjects