local ActionRequest = {}

local ACTIONS_REQUEST <const> = 'Actions'
local ACTION_ENTRY <const> = 'Action'
local INFO_ENTRY <const> = 'Info'

--[[
  Check if request has actions in it.

  @return True if request has actions, false otherwise.
]]--
function ActionRequest:IsAction()
  return Request[ACTIONS_REQUEST] ~= nil
end

--[[
  Get actions from request.

  @return Request's actions table.
]]--
function ActionRequest:GetActions()
  if (self:IsAction()) then
    return Request[ACTIONS_REQUEST]
  end
end

--[[
  Check if button with ID `buttonID` was pressed by client.

  @param[in] buttonID Button's unique ID.
  @return True if button with given ID was pressed, false otherwise.
]]--
function ActionRequest:IsButtonPressed(buttonID)
  if (self:IsAction() and Helper:CheckIfArray(self:GetActions())) then
    for _, v in ipairs(self:GetActions()) do
      if (v[ACTION_ENTRY] == 'ButtonPressed' and v[INFO_ENTRY] == buttonID) then
        return true
      end
    end
  end
  return false
end

return ActionRequest