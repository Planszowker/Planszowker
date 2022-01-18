--[[
This file is used to describe 'rounds'.
TODO
]]--

--[[
Function that is called when we 'roll' dice
(More precisely, it is called after we call RollEvent)
]]--
fsm['onRoll'] = function()
    -- 1) Roll dice
    --
end

fsm['onReroll'] = function()
    print('We re-rolled dice')
end

fsm['onConfirm'] = function()
    print('We confirmed roll')
end

-- Testing
print(fsm.current)
fsm:RollEvent()
print(fsm.current)
fsm:RollEvent()
print(fsm.current)
fsm:RerollEvent()
print(fsm.current)
fsm:ConfirmEvent()
print(fsm.current)
fsm:RerollEvent()
print(fsm.current)
fsm:RollEvent()
print(fsm.current)
fsm:ConfirmEvent()
print(fsm.current)

print('=======')
for ziu in pairs(fsm.events) do
    print(ziu)
end