local function RedisHget(key, field)  
    local value = redis.call('hget', key, field)
    if(value == false) then
        return '0'
    else
        return value
    end
end 

local domainMoidStr = ARGV[1]
local domain_moid_confs_Key = "domain/" .. domainMoidStr .. "/confs"

--exam e164 whether in the domain
local searchedConfE164 = ARGV[2]
local bIsConfInDomain = redis.call('hexists', domain_moid_confs_Key, searchedConfE164)
if(bIsConfInDomain == 0) then
    redis.log(redis.LOG_WARNING, "conf:" .. searchedConfE164 .. " not in the domain")
    return nil
end

--get conf info
local conf_e164_key = "conf/" .. searchedConfE164
local isConfExist = redis.call('exists', conf_e164_key)
if( isConfExist == 0 ) then
    redis.log(redis.LOG_WARNING, "conf:" .. searchedConfE164 .. " not exist")
    return nil
end

local confInfo = redis.call('hgetall', conf_e164_key)
local detailConfInfo = {}
for index = 1, #confInfo, 2 do
    detailConfInfo[confInfo[index]] = confInfo[index + 1]
end

local conf_e164_mix_mixid_key = conf_e164_key .. "/mix/1"
detailConfInfo["mixmode"] = RedisHget(conf_e164_mix_mixid_key, "mixmode")

local confInfoJsonStr = cjson.encode(detailConfInfo)
redis.log(redis.LOG_DEBUG, confInfoJsonStr)
return confInfoJsonStr
