local ReplyModule = {}

-- Function to add generic reply to existing table
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

-- Function to send event
function ReplyModule:ReportEvent(eventString)
  Reply['Events'] = Reply['Events'] or {}

  local eventCount = #Reply['Events']

  Reply['Events'][eventCount + 1] = {}
  Reply['Events'][eventCount + 1]['EventString'] = eventString
end

-- Function that sends resulting JSON string to players, so they can update their view.
function ReplyModule:SendReply()
  self:GenerateGenericReply(Reply)

  -- Encode reply as a JSON string
  replyString = Json.encode(Reply)

  -- Send reply to players
  SendReply(replyString)
end

-- Function to set entity's texture
function ReplyModule:SetTexture(id, texture)
  if Reply['Actions'] == nil then
    Reply['Actions'] = {}
  end

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

return ReplyModule