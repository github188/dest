local moid,mte164,onlypublic,creatormoid,admin, start, count = ARGV[1],ARGV[2],ARGV[3],ARGV[4],ARGV[5], ARGV[6], ARGV[7]
local running = '2'
local public = '1'
local hidden = '0'

local result = {success = 1}

require("tojson.lua")
require("_getconflist.lua")

getconflist(moid, start, count, result)
return tojson(result)