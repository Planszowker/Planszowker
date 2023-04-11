local ActionRequest = {}

function ActionRequest:IsAction()
  return Request['ActionRequests'] ~= nil
end

function ActionRequest:GetActions()
  if (self:IsAction()) then
    return Request['ActionRequests']
  end
end

--[[
Check if button with ID `buttonID` was pressed by client.
]]--
function ActionRequest:IsButtonPressed(buttonID)
  if (self:IsAction() and Helper:CheckIfArray(self:GetActions())) then
    for _, v in ipairs(self:GetActions()) do
      if (v['Action'] == 'ButtonPressed' and v['Info'] == buttonID) then
        return true
      end
    end
  end
  return false
end

return ActionRequest