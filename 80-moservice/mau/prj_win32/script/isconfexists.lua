--删除会议
--2015/12/24
local e164 = ARGV[1]
-- 检测会议归属
local key = 'confex/'..e164
return redis.call('exists', key)