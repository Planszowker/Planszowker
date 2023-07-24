local GameObjects = {}

-- Parse BoardDescription into LUA table
local boardDescription = Json.decode(BoardDescriptionString)
boardDescription['Entities'] = boardDescription['Entities'] or {}
boardDescription['DestinationPoints'] = boardDescription['DestinationPoints'] or {}
boardDescription['ActionBar'] = boardDescription['ActionBar'] or {}

-- Parse Entities
GameObjects['Entities'] = {}
for _, v in ipairs(boardDescription['Entities']) do
    print("[LUA] Adding entity with ID "..v['ID'])
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

-- Parse ActionBar
-- For now, we only have buttons in ActionBar
GameObjects['ActionButtons'] = {}
for _, v in ipairs(boardDescription['ActionBar']) do
    if v['Type'] == "Button" then
        GameObjects['ActionButtons'][v['ID']] = ActionButton:new(
                v['ID'],
                v['Visible']
        )
    end
end

return GameObjects