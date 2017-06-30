local moid,mte164,onlypublic,confe164 = ARGV[1],ARGV[2],ARGV[3],ARGV[4]

local result = {success = 1}

require("tojson.lua")
require("_getcascades.lua")

getcascades(confe164, result)

return tojson(result)