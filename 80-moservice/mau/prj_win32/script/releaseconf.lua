--删除会议
--2015/12/24

local e164, moid, cmuip = ARGV[1], ARGV[2], ARGV[3]

redis.call('srem', 'cmu/'..cmuip..'/confs', e164)
-- 检测会议归属
local key = 'confex/'..e164
local ip = redis.call('hget', key, 'cmu')
local needlicense = redis.call('hget', key, 'needlicense')
local confScale = redis.call('hget', key, 'maxjoinedmt')

if( ip == cmuip ) then
	redis.call('del', key, e164, 1)	
	redis.call('hdel', 'domain/'..moid..'/confs', e164)
	-- 仅为所属MCU时，才减license
    if(needlicense == '1') then
        if(tonumber(confScale) <= 8) then
            redis.call('hincrby', 'domain/'..moid..'/license',
                        'licensedsmallconfnum', -1)
        else
            redis.call('hincrby', 'domain/'..moid..'/license',
                        'licensedlargeconfnum', -1)
        end
    else
        redis.call('hincrby', 'domain/'..moid..'/license',
                    'unlicensedconfnum', -1)
    end

	redis.call('del', 'conf/'..e164)
	local confkeys = redis.call('keys', 'conf/'..e164..'/*')
	for i, delkey in ipairs(confkeys) do
		redis.call('del', delkey)
	end
	redis.call('srem', 'confs', e164)

	--推送
	local notif = {}
	notif['confs'] = 'update'
    notif['moid'] = moid
	notif['conf/'..e164] = 'delete'
	notif['domain/'..moid..'/confs'] = 'update'
	local res = redis.call('publish', 'notify', cjson.encode(notif))
    redis.log(redis.LOG_WARNING, "notify res: " .. res .. cjson.encode(notif))
end
