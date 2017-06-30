--ÐÞ¸Ä»áÒé×´Ì¬
--2015/12/24
local moid, e164, state = ARGV[1], ARGV[2], ARGV[3]
local key = 'domain/'..moid..'/confs'
redis.call('hset', key, e164, state)
return redis.call('hset', 'confex/'..e164, 'state', state)