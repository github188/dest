
--confs
local checkStatus = ""
checkStatus = checkStatus .. "Confs:\n"
local allConfs = redis.call('smembers', 'confs')
for allConfIndex, allConfE164 in ipairs(allConfs) do
    checkStatus = checkStatus .. 'confE164: ' .. allConfE164 .. '\n'
end

checkStatus = checkStatus .. '...............\n'

--cmu/cmuIp/confs
local cmuList = redis.call('keys', 'cmu/*/confs')
for cmuIndex, cmuConfKey in ipairs(cmuList) do
    checkStatus = checkStatus .. cmuConfKey .. ' has : \n'
    local cmuConfList = redis.call('smembers', cmuConfKey)
    for cmuConfIndex, cmuConfE164 in ipairs(cmuConfList) do
        checkStatus = checkStatus .. 'confE164: ' .. cmuConfE164 .. '\n'
    end
end

checkStatus = checkStatus .. '...............\n'

--domain/moid/confs
local domain_moid_confs_key_list = redis.call('keys', 'domain/*/confs')
for domainIndex, domain_moid_confs_key in ipairs(domain_moid_confs_key_list) do
    checkStatus = checkStatus .. domain_moid_confs_key .. ' has : \n'
    local domainConfs = redis.call('hgetall', domain_moid_confs_key)
    for domainConfIndex = 1, #domainConfs, 2 do
        checkStatus = checkStatus .. 'confE164: ' .. domainConfs[domainConfIndex] .. '\n' .. "status: " .. domainConfs[domainConfIndex + 1] .. '\n'
    end
end

checkStatus = checkStatus .. '...............\n'
redis.log(redis.LOG_WARNING, checkStatus)
--[[
local check_1_status = true
for cmuIndex, cmuConfKey in ipairs(cmuList) do
    local cmuConfList = redis.call('smembers', cmuConfKey)
    for cmuConfIndex, cmuConfE164 in ipairs(cmuConfList) do
        local bCmuConfE164ExistInAllConfs = false
        for allConfIndex, allConfE164 in ipairs(allConfs) do
            if allConfE164 == cmuConfE164 then
                bCmuConfE164ExistInAllConfs = true
            end
        end
        if(bCmuConfE164ExistInAllConfs == false) then
            check_1_status = false
        end
    end
end
--]]

return checkStatus
