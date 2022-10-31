--[[
This script is run each round.
]]--

print('[LUA] Current state: ' .. StateMachine.current)

--[[
Handle state changes depending on received requests.
DiceRoller does not require any additional checking.

Note: See ActionRequest
]]--
if ActionRequest:IsButtonPressed('Roll') then
    StateMachine:RollEvent()
elseif ActionRequest:IsButtonPressed('Reroll') then
    StateMachine:RerollEvent()
elseif ActionRequest:IsButtonPressed('Confirm') then
    StateMachine:ConfirmEvent()
end

-- Define when the game ends
if GetRoundsCounter() > 3 then
    FinishGame()
end