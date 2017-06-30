-- 获取会议扩展信息
local conflist = redis.call('keys', 'confex/*')
local jsonobj = {}
for i=1, #conflist, 1 do
	jsonobj[i] = {}
	jsonobj[i]['e164'] = string.sub(conflist[i], 8, -1)
	local rlt = redis.call('hgetall', conflist[i])
	for j = 1, #rlt, 2 do
		print(rlt[j]..':'..rlt[j+1])
		jsonobj[i][rlt[j]] = rlt[j+1]
	end
end
return cjson.encode(jsonobj)
