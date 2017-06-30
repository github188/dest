
local e164 = ARGV[1]
-- 检测会议归属
local key = 'conf/'..e164
return redis.call('exists', key)
