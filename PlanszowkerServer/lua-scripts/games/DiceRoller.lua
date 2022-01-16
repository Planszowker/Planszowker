local machine = require('core.lua-state-machine')

local fsm = machine.create({
    initial = 'init',
    events = {
        { name = 'rollEvent', from = {'init', 'confirm'}, to = 'roll'},
        { name = 'rerollEvent', from = 'roll', to = 'reroll'},
        { name = 'confirmEvent', from = {'roll', 'reroll'}, to = 'confirm'},
    }
})

print(fsm.current)
fsm:rollEvent()
print(fsm.current)