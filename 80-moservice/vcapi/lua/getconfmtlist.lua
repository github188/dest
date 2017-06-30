local moid,mte164,onlypublic,confe164 = ARGV[1],ARGV[2],ARGV[3],ARGV[4]
local running = '2'
local public = '1'
local hidden = '0'

local result = {success = 1}

require("tojson.lua")
require("_getconfmtlist.lua")

repeat
    local confstate = redis.call('hget', 'domain/'..moid..'/confs', confe164)
    --如果会议不存在或不是召开状态
    if confstate == nil or confstate ~= running then
        result['success'] = 0
        result['error_code'] = 0
        break
    end
    getconfmtlist(confe164, result)
until(true)
return tojson(result)