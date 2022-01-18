--[[
This file contains states available in game.

DiceRoller is a very simple game, thus there are not much states. This game is for demonstration purposes only.

<Game_name>-states.lua has to describe states and things initialized only once.

States describe how the game should behave in general. It is up to the programmer to determine separate states or don't
use them at all.

Multiple states-machines can be created to support the cases, when the whole round is divided into smaller steps that
players do in specific order.
--]]
machine = require('lua-scripts.core.lua-state-machine')

fsm = machine.create({
    initial = 'Init',
    events = {
        { name = 'RollEvent', from = {'Init', 'Confirm'}, to = 'Roll' },
        { name = 'RerollEvent', from = 'Roll', to = 'Reroll' },
        { name = 'ConfirmEvent', from = {'Roll', 'Reroll'}, to = 'Confirm' },
    }
})