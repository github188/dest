
local e164, cmuip, cmupid, starttime, state, name
 = ARGV[1], ARGV[2], ARGV[3], ARGV[4], ARGV[5], ARGV[6]

--更改会议归属
local key = 'confex/'..e164
local oldMcuIp = redis.call('hget', key, 'cmu')
redis.call('srem', 'cmu/'..oldMcuIp..'/confs', e164)
redis.call('sadd', 'cmu/'..cmuip..'/confs', e164)

key = 'confex/'..e164
local exists = redis.call('exists', key)
redis.call(
	'hmset', key, 
	'cmu', cmuip,
	'cmupid', cmupid,
	'starttime', starttime,
	'state', state,
	'name', name)

local moid = redis.call('hget', key, 'moid')
key = 'domain/' .. moid .. '/confs'
redis.call('hset', key, e164, state)

