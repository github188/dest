local moid,mte164,onlypublic,confe164,dstip,dstport = ARGV[1],ARGV[2],ARGV[3],ARGV[4],ARGV[5],ARGV[6]
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
	
	local monitorindex = dstip..'/'..dstport
	local isExist = redis.call('sismember', 'conf/'..confe164..'/monitors', monitorindex)
	if isExist == 1 then
		getmonitorinfo(confe164, monitorindex, result)
	else
		result['success'] = 0
		result['error_code'] = 0
	end
		
until(true)
return tojson(result)