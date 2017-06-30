local moid,mte164,onlypublic,confe164,cascadesid,mtid = ARGV[1],ARGV[2],ARGV[3],ARGV[4],ARGV[5],ARGV[6]
local running = '2'
local public = '1'
local hidden = '0'

local result = {success = 1 }

require("tojson.lua")
require("getmt.lua")

repeat
    local confstate = redis.call('hget', 'domain/'..moid..'/confs', confe164)
    if confstate == nil or confstate ~= running then
        result['success'] = 0
        result['error_code'] = 0
        break
    end

    --判断终端是否在会议中
    local isMtExist = {}
    if cascadesid == '' or cascadesid == '0' then
        isMtExist = redis.call('sismember', 'conf/'..confe164..'/mts', mtid)
    else
        isMtExist = redis.call('sismember', 'conf/'..confe164..'/cascade/'..cascadesid..'/mts', mtid)
    end
    if isMtExist == 1 then
        getmt(confe164, cascadesid, mtid, result)
    else
        result['success'] = 0
        result['error_code'] = 22004
    end
until(true)
return tojson(result)