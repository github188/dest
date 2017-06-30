local function Tonumber(number)
    if(number == false) then
        return 0
    else
        return tonumber(number)
    end
end
------------------------------------------------
local domainMoidStr = ARGV[1]
local confNumType = ARGV[2]

local domain_Moid_license_Key = "domain/" .. domainMoidStr .. "/license"
if(confNumType == "licensed") then
    local smallconfnum = redis.call('hget', domain_Moid_license_Key, 'licensedsmallconfnum')
    local largeconfnum = redis.call('hget', domain_Moid_license_Key, 'licensedlargeconfnum')
    return Tonumber(smallconfnum) + Tonumber(largeconfnum)
elseif(confNumType == "unlicensed") then
    local unLicensedConfNum = redis.call('hget', domain_Moid_license_Key, 'unlicensedconfnum')
    return Tonumber(unLicensedConfNum)
elseif(confNumType == "licensed_small") then
    local smallconfnum = redis.call('hget', domain_Moid_license_Key, 'licensedsmallconfnum')
    return Tonumber(smallconfnum)
elseif(confNumType == "licensed_large") then
    local largeconfnum = redis.call('hget', domain_Moid_license_Key, 'licensedlargeconfnum')
    return Tonumber(largeconfnum)
elseif(confNumType == "maxlicensed") then
    local maxSmallConfNum = redis.call('hget', domain_Moid_license_Key,
    'maxlicensedsmallconfnum')
    local maxLargeConfNum = redis.call('hget', domain_Moid_license_Key,
    'maxlicensedlargeconfnum')
    return Tonumber(maxLargeConfNum) + Tonumber(maxSmallConfNum)
else
    redis.log(redis.LOG_WARNING, "confNumType: " .. confNumType .. " invalid")
    return nil
end

