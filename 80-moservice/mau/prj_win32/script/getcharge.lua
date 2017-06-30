local function RedisHget(key, field)  
    local value = redis.call('hget', key, field)
    if(value == false) then
        return ''
    else
        return value
    end
end 
-------------------------------------------------------------

local confE164 = ARGV[1]
local confKey = 'conf/' ..confE164
local confexKey = 'confex/' .. confE164

local charge = {}
charge['confName'] = RedisHget(confKey, 'confname')
charge['confE164'] = RedisHget(confKey, 'e164')
charge['userDomainMoid'] = RedisHget(confKey, 'moid')
charge['bandWidth'] = RedisHget(confKey, 'mediabitrate')
charge['creatorId'] = RedisHget(confKey, 'creatormoid')
charge['portMeeting'] = RedisHget(confKey, 'conftype')
charge['createType'] = RedisHget(confexKey, 'creatortype')

--[[
charge['startTime'] = RedisHget(confKey, '')
charge['endTime'] = RedisHget(confKey, '')
charge['realStartTime'] = RedisHget(confKey, '')
charge['realEndTime'] = RedisHget(confKey, '')
--]]

charge['usedCp'] = '0'
charge['usedAudmix'] = '0'
charge['usedAdapt'] = '0'
charge['usedRec'] = '0'
charge['usedTvWall'] = '0'

local chargeJson = cjson.encode(charge)
return chargeJson



