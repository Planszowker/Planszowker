print('[LUA] Kingdomino init...')

--[[
  Create State Machine
--]]
StateMachine = Machine.create(
  {
    initial = 'InitState',
    events = {
      { name = 'AttachDominoEvent', from = {'InitState', 'ChoosedDominoState'}, to = 'AttachedDominoState' },
      { name = 'ChooseDominoEvent', from = 'AttachedDominoState', to = 'ChoosedDominoState'},
    }
  }
)

-- Map entities into separate table, it will be easier to use them
Entities = GameObjects.Entities


--[[
  Callback for AttachDominoEvent

  It is called after event was fired.
--]]
StateMachine['onAttachDominoEvent'] = function()
  print('AttachDominoEvent has fired!')


end