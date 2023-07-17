local ReplyModule = {}

--[[
  Function to add generic reply to existing table:
  > Adds information about players' points.
]]
function ReplyModule:GenerateGenericReply()
  Reply['PlayersInfo'] = {}
  local i = 1

  for key, player in pairs(GetPlayers()) do
    Reply['PlayersInfo'][i] = {}
    Reply['PlayersInfo'][i]['ID'] = tostring(player)
    Reply['PlayersInfo'][i]['Points'] = GetPlayerPoints(player)

    i = i + 1
  end
end

--[[
  Function to send event.

  @param[in] eventString Event string.
]]--
function ReplyModule:ReportEvent(eventString)
  Reply['Events'] = Reply['Events'] or {}

  local eventCount = #Reply['Events']

  Reply['Events'][eventCount + 1] = {}
  Reply['Events'][eventCount + 1]['EventString'] = eventString
end

--[[
  Function to set entity's texture.

  @param[in] id Entity's ID.
  @param[in] texture New entity's texture ID.
]]--
function ReplyModule:SetTexture(id, texture)
  Reply['Actions'] = Reply['Actions'] or {}

  local newIndex = #Reply['Actions'] + 1
  Reply['Actions'][newIndex] = {}
  Reply['Actions'][newIndex]['Action'] = 'SetTexture'
  Reply['Actions'][newIndex]['Entity'] = id
  Reply['Actions'][newIndex]['Texture'] = texture

  assert(Reply['Actions'][newIndex] ~= nil)
  assert(Reply['Actions'][newIndex]['Action'] ~= nil)
  assert(Reply['Actions'][newIndex]['Entity'] ~= nil)
  assert(Reply['Actions'][newIndex]['Texture'] ~= nil)
end

--[[
  Function to set object's visibility.

  @param[in] id Object's ID.
  @param[in] visibility True if object should be visible, false otherwise.
]]--
function ReplyModule:SetVisibility(id, visibility)
  Reply['Actions'] = Reply['Actions'] or {}

  local newIndex = #Reply['Actions'] + 1
  Reply['Actions'][newIndex] = {}
  Reply['Actions'][newIndex]['Action'] = 'SetVisibility'
  Reply['Actions'][newIndex]['ObjectID'] = id
  Reply['Actions'][newIndex]['Visibility'] = visibility

  assert(Reply['Actions'][newIndex] ~= nil)
  assert(Reply['Actions'][newIndex]['Action'] ~= nil)
  assert(Reply['Actions'][newIndex]['ObjectID'] ~= nil)
  assert(Reply['Actions'][newIndex]['Visibility'] ~= nil)
end

--[[
  Function that sends resulting JSON string to players, so they can update their view.

  This method is invoked automatically in internal logic, thus it shouldn't be necessary
  to invoke it manually. But, if your game's adaptation requires multiple updates in
  single round, go ahead and use it.
]]--
function ReplyModule:SendReply()
  print('[LUA-DEBUG] SendReply method invoked')

  self:GenerateGenericReply(Reply)

  -- Encode reply as a JSON string
  replyString = Json.encode(Reply)

  -- Send reply to players
  SendReply(replyString)
end

return ReplyModule