local function IsMtInConf(mtE164, confE164)  
    local confMtsKey = 'conf/' .. confE164 .. '/mts'
    local mts = redis.call('smembers', confMtsKey)
    for mtIndex, mtId in ipairs(mts) do
         local confMtKey = 'conf/' .. confE164 .. '/mt/' .. mtId
         local confMtE164 = redis.call('hget', confMtKey, 'e164')
         if confMtE164 == mtE164 then
             return true
         end
    end
    return false
end 

--------------------------------------------------------------------
local domainMoidStr = ARGV[1]
local mtE164 = ARGV[2]
local isCheckPublic = ARGV[3]

redis.log(redis.LOG_WARNING, 'domain: ' .. domainMoidStr)
redis.log(redis.LOG_WARNING, 'mtE164: ' .. mtE164)
redis.log(redis.LOG_WARNING, 'isCheckPublic: ' .. isCheckPublic)

local domainConfsKey = 'domain/' .. domainMoidStr .. '/confs'
local domainConfAndConfStateList = redis.call('hgetall', domainConfsKey)

local domainConfList = {}
for confIndex = 1, #domainConfAndConfStateList, 2 do
    if(domainConfAndConfStateList[confIndex + 1] == '2') then
        domainConfList[#domainConfList + 1] = domainConfAndConfStateList[confIndex]
    end
end

local confInfoArry = {}
for index, confE164 in ipairs(domainConfList) do
    local confKey = "conf/" .. confE164
    local confInfo = redis.call('hgetall', confKey)
    local confInfoHash = {}
    for infoIndex = 1, #confInfo, 2 do
        confInfoHash[confInfo[infoIndex] ] = confInfo[infoIndex + 1]
        redis.log(redis.LOG_WARNING, confInfo[infoIndex] .. ': ' .. confInfo[infoIndex + 1])
    end

    if(isCheckPublic == '1' and confInfoHash['publicconf'] == '0') then
        redis.log(redis.LOG_WARNING, "check publicconf and conf not publicconf")
    elseif(confInfoHash['openmode'] == '0' and IsMtInConf(mtE164, confE164) == false) then
        redis.log(redis.LOG_WARNING, "openmode == 0 and mt inexistense in conf: " .. confE164)
    else
        --get conf own mt
        local confMtKey = confKey .. "/mts"
        local confOwnMtNum = redis.call('scard', confMtKey)
        confInfoHash["mtnum"] = tostring(confOwnMtNum)

        confInfoArry[#confInfoArry + 1] = confInfoHash
    end
end

return cjson.encode(confInfoArry)
