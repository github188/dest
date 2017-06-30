local moid,mte164,onlypublic,confe164,hduid = ARGV[1],ARGV[2],ARGV[3],ARGV[4],ARGV[5]
local running = '2'
local public = '1'
local hidden = '0'

local result = {success = 1}

require("tojson.lua")
require("_gethduvmps.lua")

repeat
    local confstate = redis.call('hget', 'domain/'..moid..'/confs', confe164)
    if confstate == nil or confstate ~= running then
        result['success'] = 0
        result['error_code'] = 0
        break
    end
    
    --判断电视墙id是否在列表
    local hduinfo = string.gsub(hduid, "_", "/")
    local isexist = redis.call('sismember', 'conf/'..confe164..'/tvwalls', hduinfo)
    if isexist == 1 then
        gethduvmps(confe164, hduinfo, result)
    else
        result['success'] = 0
        result['error_code'] = 0
    end

until(true)
return tojson(result)