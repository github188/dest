local moid,mte164,onlypublic,confe164,vmpid = ARGV[1],ARGV[2],ARGV[3],ARGV[4],ARGV[5]
local running = '2'
local public = '1'
local hidden = '0'

local result = {success = 1}

require("tojson.lua")
require("_getvmpinfo.lua")

repeat
    local confstate = redis.call('hget', 'domain/'..moid..'/confs', confe164)
    if confstate == nil or confstate ~= running then
        result['success'] = 0
        result['error_code'] = 0
        break
    end

    getvmpinfo(confe164, vmpid, result)

until(true)
return tojson(result)