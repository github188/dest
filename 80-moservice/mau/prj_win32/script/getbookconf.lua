
local confE164  = ARGV[1]

local isBookConfExist = redis.call('exists', 'bookconf/' .. confE164)
if( isBookConfExist == 0 ) then
    return nil
end

local bookConfInfo = redis.call('hgetall', 'bookconf/' .. confE164)
local bookConf = {}
for infoIndex = 1,  #bookConfInfo, 2 do
    bookConf[bookConfInfo[infoIndex]] = bookConfInfo[infoIndex + 1]
end

local bookConfInsoJson = cjson.encode(bookConf)
return bookConfInsoJson
