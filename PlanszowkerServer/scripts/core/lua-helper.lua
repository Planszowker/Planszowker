local Helper = {}

--[[
Check if given variable is an array
]]--
function Helper:CheckIfArray(v)
  if (v == nil or v[1] == nil) then
    return false
  end

  local i = 0
  for _ in pairs(v) do
    i = i + 1
    if v[i] == nil then
      return false
    end
  end
  return true
end

return Helper