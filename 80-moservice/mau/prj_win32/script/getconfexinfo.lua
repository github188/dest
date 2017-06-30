-- 获取会议扩展信息
local e164 = ARGV[1]
local key = 'confex/'..e164
local rlt = redis.call('exists', key)
if rlt ~= 0 then
	local rlt = redis.call('hgetall', key)
	local obj = {}
	obj['e164'] = e164
	for i = 1, #rlt, 2 do
		obj[rlt[i]] = rlt[i+1]
	end
	return cjson.encode(obj)
end
return nil

