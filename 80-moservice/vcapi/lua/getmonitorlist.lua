local moid,mte164,onlypublic,confe164 = ARGV[1],ARGV[2],ARGV[3],ARGV[4]
local running = '2'
local public = '1'
local hidden = '0'

local result = {success = 1}

require("tojson.lua")
require("_getmonitorinfo.lua")

repeat
    local confstate = redis.call('hget', 'domain/'..moid..'/confs', confe164)
    if confstate == nil or confstate ~= running then
        result['success'] = 0
        result['error_code'] = 0
        break
    end
    
    result['monitors'] = {}
    setmetatable(result['monitors'], cjson.as_array)
    local monitors = redis.call('smembers', 'conf/'..confe164..'/monitors')
    if #monitors ~= 0 then
        for index=1, #monitors do
            result['monitors'][index] = {}
            getmonitorinfo(confe164, monitors[index], result['monitors'][index])
        end
    end
until(true)
return tojson(result)