--[[
This file contains states available in game and everything that is initialized once.

DiceRoller is a very simple game, thus there are not many states. This game is for demonstration purposes only.

<Game_name>-init.lua has to describe states and things initialized only once.

States describe how the game should behave in general. It is up to the programmer to determine separate states or don't
use them at all.

Callback from changing states are convenient way to describe how game should behave in such case.
--]]
fsm = machine.create({
    initial = 'Init',
    events = {
        { name = 'RollEvent', from = {'Init', 'Confirm'}, to = 'Roll' },
        { name = 'RerollEvent', from = 'Roll', to = 'Reroll' },
        { name = 'ConfirmEvent', from = {'Roll', 'Reroll'}, to = 'Confirm' },
    }
})

--[[
We want to use server's RNG, thus we need to create one.
]]--
dice_rng = rng.new(1,6)

--[[
Rolled dice are stored in global variable to be accessible between callbacks. This table's value is then read in the
`confirm` state.
]]--
rolledDice = {}

--[[
Function that is called when we 'roll' dice
(More precisely, it is called after we call RollEvent)

User rolls 3 dice and gets the result. Then, one can `confirm` the rolls or do a reroll, if the score was too low.
]]--
fsm['onRoll'] = function()
    print('[LUA] onRoll was invoked')
    rolledDice = {}
    for i = 1, 3 do
        rolledDice[i] = dice_rng:generateRandomNumber()
    end

    print('[LUA] Client ' .. GetCurrentPlayer() .. ' rolled '
            .. rolledDice[1] .. ' '
            .. rolledDice[2] .. ' '
            .. rolledDice[3])
end

--[[
Function that is called when we 'reroll' dice
]]--
fsm['onReroll'] = function()
    print('[LUA] onReroll was invoked')
    rolledDice = {}
    for i = 1, 3 do
        rolledDice[i] = dice_rng:generateRandomNumber()
    end

    print('[LUA] Client ' .. GetCurrentPlayer() .. ' rerolled '
            .. rolledDice[1] .. ' '
            .. rolledDice[2] .. ' '
            .. rolledDice[3])

    -- Force `confirm`, because user cannot do anything else here
    fsm:ConfirmEvent()
end

--[[
Function that is called when we 'confirm' rolls
]]--
fsm['onConfirm'] = function()
    print('[LUA] onConfirm was invoked')

    for i = 1, 3 do
        AddPointsToCurrentPlayer(rolledDice[i])
    end

    print('[LUA] Player ID ' .. GetCurrentPlayer() .. ' has ' .. GetCurrentPlayerPoints() .. ' points')

    AdvanceRound()
end