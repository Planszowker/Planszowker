--[[
This file contains states available in game and everything that is initialized once.

DiceRoller is a very simple game, thus there are not many states. This game is for demonstration purposes only.

<Game_name>-init.lua has to describe states and things initialized only once.

States describe how the game should behave in general. It is up to the programmer to determine separate states or don't
use them at all.

Callback from changing states are convenient way to describe how game should behave in such case.
--]]

StateMachine = Machine.create(
  {
    initial = 'Init',
    events = {
      { name = 'RollEvent', from = {'Init', 'Confirm'}, to = 'Roll' },
      { name = 'RerollEvent', from = 'Roll', to = 'Reroll' },
      { name = 'ConfirmEvent', from = {'Roll', 'Reroll'}, to = 'Confirm' },
    }
  }
)

-- Map entities into separate table, it will be easier to use them
Entities = GameObjects.Entities

-- We want to use server's RNG, thus we need to create one.
DiceRng = Rng.new(1,6)

--[[
  Rolled dice are stored in global variable to be accessible between callbacks. This table's value is then read in the
  `confirm` state.
]]--
RolledDice = {}

-- Function to update Dice according to results
function _updateTextures()
  for i, die in ipairs(RolledDice) do
    Entities['Die'..tostring(i)]:SetTexture('Die'..die..'.png')
  end
end

--[[
  Function to update Action Bar buttons' visibility depending
  on possible state change.
]]--
local inspect = require('scripts.core.debug.inspect')
function _updateButtonVisibility()
  for eventName, _ in pairs(StateMachine.events) do
    -- We remove `Event` from event's name - this gives us the button's name
    -- e.g. we have a button called `Roll` and event `RollEvent`.
    local buttonID = eventName:gsub("Event", "", 1)

    -- We update button's visibility based of possible translations.
    -- E.g. if user can fire event from `RollEvent` to both `RerollEvent` and
    -- `ConfirmEvent`, we update `Roll` and `Confirm` buttons' visibility.
    ReplyModule:SetVisibility(buttonID, StateMachine:can(eventName))
  end
end

--[[
  Function that is called when we 'roll' dice
  (More precisely, it is called after we call RollEvent)

  User rolls 3 dice and gets the result. Then, one can `confirm` the rolls or do a `reroll`, if the score was too low.
]]--
StateMachine['onRoll'] = function()
    RolledDice = {}
    for i = 1, 3 do
        RolledDice[i] = DiceRng:GenerateRandomNumber()
    end

    local eventString = '[LUA] Client ' .. GetCurrentPlayer() .. ' rolled '
                        .. RolledDice[1] .. ' '
                        .. RolledDice[2] .. ' '
                        .. RolledDice[3]

    ReplyModule:ReportEvent(eventString)

    -- Update textures
    _updateTextures()

    -- Update Action Bar buttons' visibility
    _updateButtonVisibility()
end

--[[
  Function that is called when we 'reroll' dice
]]--
StateMachine['onReroll'] = function()
  RolledDice = {}
  for i = 1, 3 do
    RolledDice[i] = DiceRng:GenerateRandomNumber()
  end

  local eventString = '[LUA] Client ' .. GetCurrentPlayer() .. ' rerolled '
                      .. RolledDice[1] .. ' '
                      .. RolledDice[2] .. ' '
                      .. RolledDice[3]

  ReplyModule:ReportEvent(eventString)

  _updateTextures()
  _updateButtonVisibility()

  -- Force `confirm`, because user cannot do anything else here
  StateMachine:ConfirmEvent()
end

--[[
  Function that is called when we 'confirm' rolls
]]--
StateMachine['onConfirm'] = function()
  for i = 1, 3 do
    AddPointsToCurrentPlayer(RolledDice[i])
  end

  print('[LUA] Player ID ' .. GetCurrentPlayer() .. ' has ' .. GetCurrentPlayerPoints() .. ' points')

  _updateButtonVisibility()

  AdvanceRound()
end