local ActionRequest = {}

local ACTIONS_REQUEST <const> = 'Actions'
local ACTION_ENTRY <const> = 'Action'
local OBJECT_ID <const> = 'ID'

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
  Check if object with ID `objectID` was pressed by client.

  @param[in] objectID Object's unique ID.
  @return True if object with given ID was pressed, false otherwise.
]]--
function ActionRequest:IsObjectPressed(objectID)
  if (self:IsAction() and Helper:CheckIfArray(self:GetActions())) then
    print("[LUA-DEBUG] Action Requests received")
    for _, v in ipairs(self:GetActions()) do
      print("[LUA-DEBUG] Action Request: " .. v[ACTION_ENTRY] .. ", ID: " .. v[OBJECT_ID])
      if (v[ACTION_ENTRY] == 'ObjectPressed' and v[OBJECT_ID] == objectID) then
        print("[LUA-DEBUG] Returning true")
        return true
      end
    end
  end
  return false
end

return ActionRequest