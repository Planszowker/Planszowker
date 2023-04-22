local ActionRequest = {}

local ACTIONS_REQUEST <const> = 'Actions'
local ACTION_ENTRY <const> = 'Action'
local INFO_ENTRY <const> = 'Info'

function ActionRequest:IsAction()
  return Request[ACTIONS_REQUEST] ~= nil
end

function ActionRequest:GetActions()
  if (self:IsAction()) then
    return Request[ACTIONS_REQUEST]
  end
end

--[[
Check if button with ID `buttonID` was pressed by client.
]]--
function ActionRequest:IsButtonPressed(buttonID)
  if (self:IsAction() and Helper:CheckIfArray(self:GetActions())) then
    for _, v in ipairs(self:GetActions()) do
      print('=-=-=-=-=-=-=')
      print(_)
      print(v)
      if (v[ACTION_ENTRY] == 'ButtonPressed' and v[INFO_ENTRY] == buttonID) then
        return true
      end
    end
  end
  return false
end

return ActionRequest