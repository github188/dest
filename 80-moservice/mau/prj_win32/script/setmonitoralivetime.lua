
local e164 = ARGV[1]
local ip = ARGV[2]
local port = ARGV[3]
local time = ARGV[4]
local key = 'conf/'..e164..'/monitor/'..ip..'/'..port
local exists = redis.call('exists', key)
if(exists == 1) then
    redis.call('hset', key, 'alivetime', time)
end
