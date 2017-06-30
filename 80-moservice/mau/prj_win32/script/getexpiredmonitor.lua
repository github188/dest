
local curtime = ARGV[1]

local expiredtime = 30

local result = {}
local confs = redis.call('smembers', 'confs')
for idx = 1, #confs do
	local cmuip = redis.call('hget', 'confex/'..confs[idx], 'cmu')
	local monitors = redis.call('smembers', 'conf/' .. confs[idx] .. '/monitors')
	for i = 1, #monitors do
		local key = 'conf/' .. confs[idx] .. '/monitor/' .. monitors[i]
		local monitor = redis.call('hgetall', key)
		local monitorhash = {}
		for j = 1, #monitor, 2 do
			monitorhash[monitor[j]] = monitor[j + 1]
		end
		if monitorhash['alivetime'] == nil then
			redis.call('hset', key, 'alivetime', curtime)
		else	
			local timediff = tonumber(curtime) - tonumber(monitorhash['alivetime'])
			if timediff > expiredtime then
				monitorhash['mcuip'] = cmuip
				monitorhash['e164'] = confs[idx]
				result[#result + 1] = monitorhash
			end
		end
	end
end
if next(result) == nil then
	return '[]'
else
	return cjson.encode(result)
end
