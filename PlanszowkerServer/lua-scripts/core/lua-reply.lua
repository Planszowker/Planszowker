json = require("lua-json")

local replyModule = {}

function replyModule.convertReply(replyTable)
  json.encode(replyTable)
end

return replyModule