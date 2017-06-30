
local domainMOID = ARGV[1]

local bookConfE164List = redis.call('smembers', 'bookconfs')
local bookConfInfoList = {}
for e164Index, e164 in ipairs(bookConfE164List) do
    local bookConfMOID = redis.call('hget', 'bookconf/' .. e164, 'moid')
    if(domainMOID == bookConfMOID) then
        local confInfoList = {}
        local bookConfInfo = redis.call('hgetall', 'bookconf/' .. e164)
        for infoIndex = 1,  #bookConfInfo, 2 do
            confInfoList[bookConfInfo[infoIndex]] = bookConfInfo[infoIndex + 1]
        end
        bookConfInfoList[#bookConfInfoList + 1] = confInfoList
    end
end

local bookConfListJson = cjson.encode(bookConfInfoList)
return bookConfListJson
