
local meetingID = ARGV[1]

local bookConfE164List = redis.call('smembers', 'bookconfs')
for e164Index, e164 in ipairs(bookConfE164List) do
    local redisMeetingID = redis.call('hget', 'bookconf/' .. e164, 'meetingid')
    if (redisMeetingID == meetingID) then
        return redis.call('hget', 'bookconf/' .. e164, 'e164')
    end
end

return nil
