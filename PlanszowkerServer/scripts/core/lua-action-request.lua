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
Check if button with UniqueName `buttonName` was pressed by client.
]]--
function ActionRequest:IsButtonPressed(buttonName)
  if (self:IsAction() and Helper:CheckIfArray(self:GetActions())) then
    for _, v in ipairs(self:GetActions()) do
      if (v['Action'] == 'ButtonPress' and v['Info'] == buttonName) then
        return true
      end
    end
  end
  return false
end

return ActionRequest