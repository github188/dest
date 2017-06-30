
local conflist = redis.call('keys', 'confex/*')
for i=1, #conflist, 1 do
    if redis.call('hget', conflist[i], 'state') ~= '1' then
        redis.call('hset', conflist[i], 'state', 3)
    end
end

local domainList = redis.call('keys', 'domain/*/confs')
for i=1, #domainList, 1 do
    local domainConfs = redis.call('hgetall', domainList[i])
    for j=1, #domainConfs, 2 do
        if redis.call('hget', domainList[i], domainConfs[j]) ~= '1' then
            redis.call('hset', domainList[i], domainConfs[j], 3)
        end
    end
end

