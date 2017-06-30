
local confE164 = ARGV[1]
local confName = ARGV[2]
local meetingID = ARGV[3]
local bookStartTime = ARGV[4]
local bookEndTime = ARGV[5]
local bookConfBelongDomainMOID = ARGV[6]
local confDetailJson = ARGV[7]
local password = ARGV[8]
local isPublicConf = ARGV[9]
local openMode = ARGV[10]
local mtNum = ARGV[11]
local bitRate = ARGV[12]
local resolution = ARGV[13]
local satMode = ARGV[14]
local dualMode = ARGV[15]
local bNoDisturb = ARGV[16]
local maxJoinedMtNum = ARGV[17]
local creatorName = ARGV[18]
local creatorAccount = ARGV[19]
local creatorAccountType = ARGV[20]
local creatorTelephone = ARGV[21]
local creatorMobile = ARGV[22]
local mixMode = ARGV[23]
local mediatype = ARGV[24]

local bookconfs_key = 'bookconfs'
redis.call('sadd', bookconfs_key, confE164)

local bookconf_e164_key = 'bookconf/' .. confE164
redis.call('hmset', bookconf_e164_key,
            'e164', confE164,
            'confname', confName,
            'meetingid', meetingID,
            'starttime', bookStartTime,
            'endtime', bookEndTime,
            'moid', bookConfBelongDomainMOID,
            'detailjson', confDetailJson,
            'password', password,
            'publicconf', isPublicConf,
            'openmode', openMode,
            'mtnum', mtNum,
            'bitrate', bitRate,
            'resolution', resolution,
            'sat', satMode,
            'dualmode', dualMode,
            'nodisturb', bNoDisturb,
            'maxjoinedmt', maxJoinedMtNum,
            'creatorname', creatorName,
            'creatoraccount', creatorAccount,
            'creatoraccounttype', creatorAccountType,
            'creatortelephone', creatorTelephone,
            'creatormobile', creatorMobile,
            'mixmode', mixMode,
            'mediatype', mediatype)

