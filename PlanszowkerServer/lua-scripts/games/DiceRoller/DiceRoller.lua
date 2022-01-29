--[[
This script is run each round.
]]--

print("[LUA] Request...")
print(request)
print("===")
print(request["Type"])
print(request["Action"])
print(request["Name"])
print("===")

print("Current")
print(fsm.current);

--[[
Handle state changes depending on received requests.
DiceRoller does not require any additional checking other than state
]]--
if request["Action"] == "ButtonPress" then
    if request["Name"] == "Roll" then
        fsm:RollEvent()
    elseif request["Name"] == "Reroll" then
        fsm:RerollEvent()
    elseif request["Name"] == "Confirm" then
        fsm:Confirm()
    end
end

fufu={}
fufu["asd"] = "asdasdasdasd"
fufu["ziu"] = "ziuizuizuuiz"
fufu["gowno"] = 123

-- sendReply(fufu)

-- Define when the game ends
if GetRoundsCounter() > 3 then
    FinishGame()
end