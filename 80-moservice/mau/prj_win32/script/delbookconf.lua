
local confE164 = ARGV[1]

redis.call('srem', 'bookconfs', confE164)
redis.call('del', 'bookconf/' .. confE164)
