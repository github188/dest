--创建会议
--2015/12/24
local e164, moid, cmuip, cmupid, starttime, booktime, duration, needlicense, state, name, meetingid, creatortype, confScale, creatorName, creatorAccount, creatorAccountType, creatorTelephone, creatorMobile, realstarttime
 = ARGV[1], ARGV[2], ARGV[3], ARGV[4], ARGV[5], ARGV[6], ARGV[7], ARGV[8], ARGV[9], ARGV[10], ARGV[11], ARGV[12], ARGV[13], ARGV[14], ARGV[15],
   ARGV[16], ARGV[17], ARGV[18], ARGV[19]
local key = 'domain/'..moid..'/confs'
redis.call('hset', key, e164, state)
key = 'confex/'..e164
local exists = redis.call('exists', key)
redis.call(
	'hmset', key, 
	'cmu', cmuip,
	'cmupid', cmupid,
	'starttime', starttime, 
	'booktime', booktime,
	'duration', duration,
	'moid', moid,
	'needlicense', needlicense,
	'state', state,
	'name', name,
	'meetingid', meetingid,
    'creatortype', creatortype,
    'maxjoinedmt', confScale,
    'creatorname', creatorName,
    'creatoraccount', creatorAccount,
    'creatoraccounttype', creatorAccountType,
    'creatortelephone', creatorTelephone,
    'creatormobile', creatorMobile,
	'realstarttime', realstarttime) 

key = 'cmu/'..cmuip..'/confs'	
redis.call('sadd', key, e164)

--第一次创建，自增license
if (exists == 0) then
	key = 'domain/' .. moid .. '/license'
    if(needlicense == '1') then
        if(tonumber(confScale) <= 8) then
            redis.call('hincrby', key, 'licensedsmallconfnum', 1)
        else
            redis.call('hincrby', key, 'licensedlargeconfnum', 1)
        end
    else
        redis.call('hincrby', key, 'unlicensedconfnum', 1)
    end
end
