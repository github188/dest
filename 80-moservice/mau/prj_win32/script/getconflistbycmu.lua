local cmuip = ARGV[1]
local conflist = redis.call('smembers', 'cmu/'..cmuip..'/confs')
local jsonobj = {}
for i=1, #conflist, 1 do
	jsonobj[i] = {}
	jsonobj[i]['e164'] = conflist[i]
	local rlt = redis.call('hgetall', 'confex/'..conflist[i])
	for j = 1, #rlt, 2 do
		print(rlt[j]..':'..rlt[j+1])
		jsonobj[i][rlt[j]] = rlt[j+1]
	end
end
return cjson.encode(jsonobj)
