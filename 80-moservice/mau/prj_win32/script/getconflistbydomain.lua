
local domainMoidStr = ARGV[1]

local domainConfsKey = 'domain/' .. domainMoidStr .. '/confs'
local domainConfAndConfStateList = redis.call('hgetall', domainConfsKey)

local domainConfList = {}
for confIndex = 1, #domainConfAndConfStateList, 2 do
    domainConfList[#domainConfList + 1] = domainConfAndConfStateList[confIndex]
end

local confInfoArry = {}
for index, confE164 in ipairs(domainConfList) do
    confInfoArry[index] = {}
    local rlt = redis.call('hgetall', 'confex/'.. confE164)
    confInfoArry[index]['e164'] = confE164
    for j = 1, #rlt, 2 do
        print(rlt[j]..':'..rlt[j+1])
        confInfoArry[index][rlt[j] ] = rlt[j+1]
    end
end
return cjson.encode(confInfoArry)
