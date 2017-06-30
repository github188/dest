--¹¹ÔìE164ºÅÂë
local prefix, beginno, endno = ARGV[1], ARGV[2], ARGV[3]
beginno = tonumber(beginno)
endno = tonumber(endno)
local finde164 = false
local confe164
local curno = 0

local rlt = redis.call('hget', 'static/conf', 'serialno')
if rlt then
	curno = tonumber(rlt)
end
for i=beginno,endno do
	if(curno > endno) then
		curno = beginno
	end
	confe164 = string.format("%s%04d", prefix, curno)
	local isexists = (redis.call('exists', 'confex/'..confe164)
                    or redis.call('exists', 'bookconf/'..confe164))
	curno = curno+1
	if (isexists == 0) then
		redis.call('hset', 'static/conf', 'serialno', curno)
		finde164 = true
		break
	end
end

if finde164 then
	return confe164
else
	return ''
end
