--
-- User: zoujunlong
-- Date: 2016/4/28
-- Time: 11:15
--
local moid,mte164,onlypublic,creatormoid,confe164,admin = ARGV[1],ARGV[2],ARGV[3],ARGV[4],ARGV[5],ARGV[6]
local running = '2'
local public = '1'
local hidden = '0'

local result = {success = 1}

require("tojson.lua")
require("_getconf.lua")

repeat
    local confstate = redis.call('hget', 'domain/'..moid..'/confs', confe164)
    -- 如果会议不存在或不是召开状态
    if confstate == nil or confstate ~= running then
        result['success'] = 0
        result['error_code'] = 0
        break
    end
    
    local confinfo = redis.call('hgetall', 'conf/'..confe164)
    local confhash = {}
    for j=1, #confinfo, 2 do
        confhash[confinfo[j]] = confinfo[j+1]
    end

    if onlypublic == 1 and confhash['publicconf'] ~= public then break end
    if admin ~= '1' and creatormoid ~= confhash['creatormoid'] and confhash['openmode'] == hidden then
        local mtlist = redis.call('smembers', 'conf/'..confe164..'/mts')
        local find = false
        for i,mtid in ipairs(mtlist) do
            local e164 = redis.call('hget', 'conf/'..confe164..'/mt/'..mtid, 'e164')
            if mte164 == e164 then
                find = true
                break
            end
        end
        if not find then break end
    end
    getconf(confe164, result)
until(true)
return tojson(result)
