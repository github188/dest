-------------------------------------------------------------
local function RedisHget(key, field)  
    local value = redis.call('hget', key, field)
    if(value == false) then
        return ''
    else
        return value
    end
end 

local function GetPollAttr(confKey, pollParamKey)
    local pollParam = {}
    pollParam['pollmode'] = RedisHget(pollParamKey, 'pollmode')
    pollParam['pollstate'] = RedisHget(pollParamKey, 'pollstate')
    pollParam['pollnum'] = RedisHget(pollParamKey, 'pollnum')
    pollParam['keeptime'] = RedisHget(pollParamKey, 'keeptime')
    pollParam['pollindex'] = RedisHget(pollParamKey, 'curidx')

    local pollMtNum = redis.call('hget', pollParamKey, 'mtnum')
    if(pollMtNum) then
        local pollMtList = {}
        for pollMtIndex = 1, pollMtNum do
            local pollMtAttr = {}
            local pollMtKey = pollParamKey .. '/member/' .. (pollMtIndex - 1)
            if(redis.call('exists', pollMtKey) == 1) then
                local mtIdentify = {}
                mtIdentify['mtid'] = RedisHget(pollMtKey, 'mtid')
                mtIdentify['mcuidx'] = RedisHget(pollMtKey, 'mtcascade')
                pollMtAttr['mtidentify'] = mtIdentify
                pollMtAttr['mtchnid'] = RedisHget(pollMtKey, 'mtchnidx')
                local confMtKey = confKey .. '/mt/' .. mtIdentify['mtid']
                pollMtAttr['mttype'] = RedisHget(confMtKey, 'mttype')
                pollMtList[#pollMtList + 1] = pollMtAttr
            end
        end
        pollParam['pollmt'] = pollMtList
    end
    return pollParam
end

local function GetHduIDAndChnIdx(hduIDAndChnIdx)
    local i, j = string.find(hduIDAndChnIdx, '/')
    if(i ~= nil and i > 1) then
        local hduID = string.sub(hduIDAndChnIdx, 1, i - 1)
        local chnIdx = string.sub(hduIDAndChnIdx, i + 1, #hduIDAndChnIdx)
        return hduID, chnIdx
    else
        return '', ''
    end
end

-------------------------------------------------------------
local confE164 = ARGV[1]
local curTime = ARGV[2]
local duration = ARGV[3]
local hasXmpu = ARGV[4]
local restoreReq = {}

local key = 'conf/'..confE164
if redis.call('exists', key) ~= 1 then
    return ''
end

redis.log(redis.LOG_WARNING, "restoreReq begin")
redis.log(redis.LOG_WARNING, 'e164: ' .. confE164)
------------------------------------------------------------
local confKey = 'conf/' .. confE164 
local confexKey = 'confex/' .. confE164
restoreReq['type'] = 'MAU_MCU_RESTORECONF_REQ'
restoreReq['confE164'] = confE164
restoreReq['ip'] = RedisHget(confexKey, 'cmu')
restoreReq['moid'] = RedisHget(confKey, 'moid')
restoreReq['confname'] = RedisHget(confKey, 'confname')
restoreReq['starttime'] = curTime
restoreReq['creatorname'] = RedisHget(confKey, 'creatorname')
restoreReq['creatormoid'] = RedisHget(confKey, 'creatormoid')
restoreReq['isportconf'] = RedisHget(confKey, 'conftype')
restoreReq['psw'] = RedisHget(confKey, 'password')
restoreReq['duration'] = duration
restoreReq['encryptmode'] = RedisHget(confKey, 'encryptmode')
restoreReq['enckeymanual'] = '1'
restoreReq['encryptkey'] = RedisHget(confKey, 'encryptkey')
restoreReq['mixmode'] = RedisHget(confKey .. '/mix/1', 'mixmode')
restoreReq['vacinterval'] = RedisHget(confKey, 'vacinterval')
restoreReq['videoprior'] = RedisHget(confKey, 'videoprior')
restoreReq['allmute'] = RedisHget(confKey, 'allmute')
restoreReq['allsilence'] = RedisHget(confKey, 'allsilence')
restoreReq['dualmode'] = RedisHget(confKey, 'dualmode')
restoreReq['speakersrc'] = RedisHget(confKey, 'speakersrc')
restoreReq['callmode'] = RedisHget(confKey, 'callmode')
restoreReq['callinterval'] = RedisHget(confKey, 'callinterval')
restoreReq['calltimes'] = RedisHget(confKey, 'calltimes')
restoreReq['publicconf'] = RedisHget(confKey, 'publicconf')
restoreReq['openmode'] = RedisHget(confKey, 'openmode')
restoreReq['nodisturb'] = RedisHget(confKey, 'nodisturb')
restoreReq['rentalmode'] = RedisHget(confKey, 'rentalmode')
restoreReq['vmpmodule'] = '1'
restoreReq['pollmodule'] = '1'
restoreReq['mixmodule'] = '1'
restoreReq['hdumodule'] = '1'
restoreReq['datamode'] = RedisHget(confKey, 'datamode')
restoreReq['onereforming'] = RedisHget(confKey, 'onereforming')
restoreReq['maxjoinedmt'] = RedisHget(confKey, 'maxjoinedmt')
restoreReq['preoccupy'] = RedisHget(confKey, 'preoccupy')
restoreReq['autoclose'] = '0'
restoreReq['hasxmpu'] = hasXmpu

local speakerMtId = redis.call('hget', confKey .. '/speaker', 'mtid')
local chairmanMtId = redis.call('hget', confKey .. '/chairman', 'mtid')
------------------------------------------------------------
local satConf = {}
------------------------------------------------------------
local dcastConf = {}
dcastConf['dcast'] = RedisHget(confKey, 'dcast')
dcastConf['dcastip'] = RedisHget(confKey, 'dcastip')
dcastConf['dcastport'] = RedisHget(confKey, 'dcastport')
restoreReq['dcastconf'] = dcastConf
------------------------------------------------------------
local cascadeConf = {}
cascadeConf['cascademode'] = RedisHget(confKey, 'cascademode')
cascadeConf['isupload'] = RedisHget(confKey, 'isupload')
cascadeConf['isspy'] = RedisHget(confKey, 'isspy')
cascadeConf['spybandwidth'] = RedisHget(confKey, 'spybandwidth')
restoreReq['cascadeconf'] = cascadeConf
------------------------------------------------------------
local videoFormat = {}
videoFormat['mediatype'] = RedisHget(confKey, 'mediatype')
videoFormat['mediabitrate'] = RedisHget(confKey, 'mediabitrate')
videoFormat['mediares'] = RedisHget(confKey, 'mediares')
videoFormat['mediah264'] = RedisHget(confKey, 'mediah264')
videoFormat['mediaframe'] = RedisHget(confKey, 'mediaframe')
local videoFormatArry = {}
videoFormatArry[#videoFormatArry + 1] = videoFormat
restoreReq['videoformat'] = videoFormatArry
------------------------------------------------------------
local mt = {}
local doubleVideoMt = {}
local mtSet = redis.call('smembers', confKey .. '/mts')
for mtIndex, mtId in ipairs(mtSet) do
    local confMtidKey = confKey .. '/mt/' .. mtId
    local mtType = RedisHget(confMtidKey, 'mttype')
    local mtIdentify = {}
    mtIdentify['mtid'] = RedisHget(confMtidKey, 'mtid')
    mtIdentify['mcuidx'] = RedisHget(confKey .. '/cascade/mt/' .. mtIdentify['mtid'], 'mtcascade')
    -- 10 is vrs
    if mtType ~= '10' then
        local mtAttr = {}
        mtAttr['mtidentify'] = mtIdentify

        mtAttr['mttype'] = mtType
        mtAttr['calltype'] = RedisHget(confMtidKey, 'calltype')
        if(mtAttr['calltype'] == '1' or mtAttr['calltype'] == '9') then
            mtAttr['mtNO'] = RedisHget(confMtidKey, 'mtip')
        elseif(mtAttr['calltype'] == '2' or mtAttr['calltype'] == '6') then
            mtAttr['mtNO'] = RedisHget(confMtidKey, 'e164')
        else
            mtAttr['mtNO'] = RedisHget(confMtidKey, 'mtalias')
        end

        if(mtId == speakerMtId) then
            restoreReq['speakeralias'] = mtAttr['mtNO']
        end
        if(mtId == chairmanMtId) then
            restoreReq['chairmanalias'] = mtAttr['mtNO']
        end

        mtAttr['protocol'] = RedisHget(confMtidKey, 'protocol')
        mtAttr['bitrate'] = RedisHget(confMtidKey, 'bitrate')
        mtAttr['invitemode'] = RedisHget(confMtidKey, 'invitemode')
        mtAttr['silence'] = RedisHget(confMtidKey, 'silence')
        mtAttr['mute'] = RedisHget(confMtidKey, 'mute')
        local mtCallMode = RedisHget(confMtidKey, 'callmode')
        if(mtCallMode == '0' and RedisHget(confMtidKey, 'online') == '1') then
            mtCallMode = '1'
        end
        mtAttr['CallMode'] = mtCallMode
        mt[#mt+1] = mtAttr
    end

    local isDualMt = RedisHget(confMtidKey, 'dual')
    if(isDualMt == '1') then
        doubleVideoMt['mtidentify'] = mtIdentify
        restoreReq['doublevideomt'] = doubleVideoMt
    end
end
restoreReq['mt'] = mt
    
------------------------------------------------------------
    
local vipMtList = {} 
local vipKey = confKey .. '/vip'
local vipNum = redis.call('llen', vipKey)
for vipIndex = 0, vipNum - 1, 1 do
    local vipMtId = redis.call('lindex', vipKey, vipIndex)
    local confMtKey = confKey .. '/mt/' .. vipMtId
    local mtIdentify = {}
    mtIdentify['mtid'] = RedisHget(confMtKey, 'mtid')
    mtIdentify['mcuidx'] = RedisHget(confKey .. '/cascade/mt/' .. mtIdentify['mtid'], 'mtcascade')
    local mtIdentifyWrap = {}
    mtIdentifyWrap['mtidentify'] = mtIdentify
    vipMtList[#vipMtList + 1] = mtIdentifyWrap
end
restoreReq['vipmt'] = vipMtList

------------------------------------------------------------
--mcu default
--local audioFormat = {}
--local secVideoFormat = {}
------------------------------------------------------------
local videoSelectList = {}
local selectInfo = redis.call('keys', confKey .. '/selected/video/*')
for selectIndex, selectKey in ipairs(selectInfo) do
    local videoSelectDstMtID = RedisHget(selectKey, 'dstid')
    if(videoSelectDstMtID == chairmanMtId) then
        local selectItem = {}
        local selectDst = {}
        local selectDstMtIdentify = {}
        selectDstMtIdentify['mtid'] = videoSelectDstMtID
        selectDstMtIdentify['mcuidx'] = RedisHget(selectKey, 'dstcascade')
        selectDst['mtidentify'] = selectDstMtIdentify
        selectDst['mtchnid'] = RedisHget(selectKey, 'dstchnidx')
        selectItem['selectdst'] = selectDst
        
        local selectSrc = {}
        local selectSrcMtIdentify = {}
        selectSrcMtIdentify['mtid'] = RedisHget(selectKey, 'srcid')
        selectSrcMtIdentify['mcuidx'] = RedisHget(selectKey, 'srccascade')
        selectSrc['mtidentify'] = selectSrcMtIdentify
        selectSrc['selectsrctype'] = RedisHget(selectKey, 'srctype')
        selectSrc['mtchnid'] = RedisHget(selectKey, 'srcchnidx')
        selectItem['selectsrc'] = selectSrc

        selectItem['switchmode'] = '1'

        videoSelectList[#videoSelectList + 1] = selectItem
        break
    end
end
redis.log(redis.LOG_WARNING, 'videoSelectList length: ' .. #videoSelectList)

local audioSelectList = {}
local audioSelectInfo = redis.call('keys', confKey .. '/selected/audio/*')
redis.log(redis.LOG_WARNING, 'autioItemCnt: ' .. #audioSelectInfo)
for selectIndex, selectKey in ipairs(audioSelectInfo) do
    local audioSelectDstMtID = RedisHget(selectKey, 'dstid')
    if(audioSelectDstMtID == chairmanMtId) then
        local selectItem = {}
        local selectDst = {}
        local selectDstMtIdentify = {}
        selectDstMtIdentify['mtid'] = audioSelectDstMtID
        selectDstMtIdentify['mcuidx'] = RedisHget(selectKey, 'dstcascade')
        selectDst['mtidentify'] = selectDstMtIdentify
        selectDst['mtchnid'] = RedisHget(selectKey, 'dstchnidx')
        selectItem['selectdst'] = selectDst
        
        local selectSrc = {}
        local selectSrcMtIdentify = {}
        selectSrcMtIdentify['mtid'] = RedisHget(selectKey, 'srcid')
        selectSrcMtIdentify['mcuidx'] = RedisHget(selectKey, 'srccascade')
        selectSrc['mtchnid'] = RedisHget(selectKey, 'srcchnidx')
        selectItem['selectsrc'] = selectSrc
        selectSrc['mtidentify'] = selectSrcMtIdentify

        selectItem['switchmode'] = '2'

        audioSelectList[#audioSelectList + 1] = selectItem
        break
    end
end

local uniqueAudioSelectList = {}
for audioSelectIndex, audioSelectVal in ipairs(audioSelectList) do
    local isVideoSelectEqualToAudioSelect = false
    for videoSelectIndex, videoSelectVal in ipairs(videoSelectList) do
        if(videoSelectVal['selectsrc']['mtidentify']['srcid'] == audioSelectVal['selectsrc']['mtidentify']['srcid']
            and videoSelectVal['selectdst']['mtidentify']['dstid'] == audioSelectVal['selectdst']['mtidentify']['dstid']) then
            isVideoSelectEqualToAudioSelect = true
            videoSelectList[videoSelectIndex]['switchmode'] = '3'
        end
    end
    if isVideoSelectEqualToAudioSelect == false then
        uniqueAudioSelectList[#uniqueAudioSelectList + 1] = audioSelectVal
    end
end
for i, audioSelectValue in ipairs(uniqueAudioSelectList) do
    videoSelectList[#videoSelectList + 1] = audioSelectValue
end

restoreReq['mtselectstatus'] = videoSelectList
------------------------------------------------------------
local vmpKey = confKey .. '/vmp/1'
if(redis.call('exists', vmpKey) == 1) then
    local vmpParam = {}
    vmpParam['vmpbrdst'] = RedisHget(vmpKey, 'vmpbrdst')
    vmpParam['vmpstyle'] = RedisHget(vmpKey, 'vmpstyle')
    vmpParam['mode'] = RedisHget(vmpKey, 'mode')
    vmpParam['batchpolltime'] = RedisHget(vmpKey, 'batchpolltime')
    vmpParam['batchpollnum'] = RedisHget(vmpKey, 'batchpollnum')
    vmpParam['vmprim'] = RedisHget(vmpKey, 'vmprim')
    vmpParam['vmpaddalias'] = RedisHget(vmpKey, 'vmpaddalias')
    vmpParam['voicehint'] = RedisHget(vmpKey, 'voicehint')

    local chnNumStr = redis.call('hget', vmpKey, 'chnnum')
    if(chnNumStr) then
        local vmpMembers = {}
        local chnNum = tonumber(chnNumStr)
        for chnIndex = 1, chnNum do
            local vmpChannel = {}
            local vmpChannelKey = vmpKey .. '/channel/' .. (chnIndex - 1)
            if(redis.call('exists', vmpChannelKey) == 1) then
                vmpChannel['chnidx'] = redis.call('hget', vmpChannelKey, 'chnidx')
                vmpChannel['vmptype'] = RedisHget(vmpChannelKey, 'vmptype')
                vmpChannel['vmpstatus'] = RedisHget(vmpChannelKey, 'vmpstatus')
                local mtIdentify = {}
                mtIdentify['mtid'] = RedisHget(vmpChannelKey, 'mtid')
                mtIdentify['mcuidx'] = RedisHget(vmpChannelKey, 'mtcascade')
                vmpChannel['mtidentify'] = mtIdentify
                vmpChannel['mttype'] = RedisHget(vmpChannelKey, 'mttype')
                vmpChannel['mtchnid'] = RedisHget(vmpChannelKey, 'mtchnidx')
                vmpMembers[#vmpMembers + 1] = vmpChannel
            end
        end
        vmpParam['vmpmembers'] = vmpMembers

        --[[
        local memberPollParam = {}
        for chnIndex = 1, chnNum do
            local vmpChannelKey = vmpKey .. '/channel/' .. (chnIndex - 1)
            local vmpPollKey = vmpKey .. '/channel/' .. (chnIndex - 1) .. '/poll'
            if(redis.call('exists', vmpPollKey) == 1) then
                local chnMemberPollParam = {}
                chnMemberPollParam['chnidx'] = redis.call('hget', vmpChannelKey, 'chnidx')
                chnMemberPollParam['pollstate'] = RedisHget(vmpPollKey, 'pollstate')
                chnMemberPollParam['pollnum'] = RedisHget(vmpPollKey, 'pollnum')
                chnMemberPollParam['keeptime'] = RedisHget(vmpPollKey, 'keeptime')
                chnMemberPollParam['pollindex'] = RedisHget(vmpPollKey, 'curidx')
                local pollMt = {}
                local mtNumStr = redis.call('hget', vmpPollKey, 'mtnum')
                if(mtNumStr) then
                    local mtNum = tonumber(mtNumStr)
                    for mtIndex = 1, mtNum, 1 do
                        local mtAttr = {}
                        local mtIdentify = {}
                        local vmpPollMtKey = vmpPollKey .. '/member/' .. mtIndex
                        mtIdentify['mtid'] = RedisHget(vmpPollMtKey, 'mtid')
                        mtIdentify['mcuidx'] = RedisHget(vmpPollMtKey, 'mtcascade')
                        mtAttr['mtidentify'] = mtIdentify
                        mtAttr['mttype'] = RedisHget(vmpPollMtKey, 'mttype')
                        mtAttr['mtchnid'] = RedisHget(vmpPollMtKey, 'mtchnidx')
                        pollMt[mtIndex] = mtAttr
                    end
                end
                chnMemberPollParam['pollmt'] = pollMt
                memberPollParam[#memberPollParam + 1] = chnMemberPollParam
            end
        end
        vmpParam['memberpollparam'] = memberPollParam
        --]]
    end
    restoreReq['vmpparam'] = vmpParam
end
------------------------------------------------------------
--//poll(confpoll, chairmanpool)
local pollParamKey = confKey .. '/poll'
if(redis.call('exists', pollParamKey) == 1) then
    restoreReq['pollparam'] = GetPollAttr(confKey, pollParamKey)
end

local chairmanPollKey = confKey .. '/chairmanpoll'
if(redis.call('exists', chairmanPollKey) == 1) then
    restoreReq['chairmanpoll'] = GetPollAttr(confKey, chairmanPollKey)
end
------------------------------------------------------------

local mixKey = confKey .. '/mix/1'
if(redis.call('exists', mixKey) == 1) then
    local mixParam = {}
    mixParam['mixmode'] = RedisHget(mixKey, 'mixmode')
    mixParam['vacinterval'] = RedisHget(mixKey, 'vacinterval')

    local mixMtNumStr = redis.call('hget', mixKey, 'mtnum')
    if(mixMtNumStr) then 
        local mixMtNum = tonumber(mixMtNumStr)
        local mixMemberList = {}
        for mixMtIndex = 1, mixMtNum do
            local mixMtKey = mixKey .. '/member/' .. (mixMtIndex - 1)
            if(redis.call('exists', mixMtKey) == 1) then
                local mtIdentify = {}
                mtIdentify['mtid'] = RedisHget(mixMtKey, 'mtid')
                mtIdentify['mcuidx'] = RedisHget(mixMtKey, 'mtcascade')
                local mixMember = {}
                mixMember['mtidentify'] = mtIdentify
                mixMember['mttype'] = RedisHget(mixMtKey, 'mttype')
                mixMember['mtchnid'] = RedisHget(mixMtKey, 'mtchnidx')
                mixMemberList[#mixMemberList + 1] = mixMember
            end
        end
        mixParam['mixmember'] = mixMemberList
    end
    restoreReq['mixparam'] = mixParam 
end

------------------------------------------------------------
--hdu
local hduChnArry = {}
local hduchns = redis.call('smembers', confKey .. '/tvwalls')
for chnIndex, hduIDAndChnIdx in ipairs(hduchns) do
    local hduChnKey = confKey .. '/tvwall/' .. hduIDAndChnIdx
    if(redis.call('exists', hduChnKey) == 1) then
        local hduChnAttr = {} 
        hduChnAttr['hduschedidx'] = RedisHget(hduChnKey, 'schedidx')
        hduChnAttr['hdustyle'] = RedisHget(hduChnKey, 'style')
        local hduID, hduChnIdx = GetHduIDAndChnIdx(hduIDAndChnIdx)
        hduChnAttr['hduid'] = hduID
        hduChnAttr['chnnlidx'] = hduChnIdx

    --[[
        local hduChnPollKey = hduChnKey .. '/poll'
        if(redis.call('exists', hduChnPollKey) == 1) then
            hduChnAttr['hdupoll'] = '1'
            hduChnAttr['pollnum'] = RedisHget(hduChnPollKey, 'pollnum')
            hduChnAttr['keeptime'] = RedisHget(hduChnPollKey, 'keeptime')
            hduChnAttr['pollmode'] = RedisHget(hduChnPollKey, 'pollmode')
            hduChnAttr['pollstate'] = RedisHget(hduChnPollKey, 'pollstate')
            hduChnAttr['pollindex'] = RedisHget(hduChnPollKey, 'curidx')
        else 
        end
    --]]
        hduChnAttr['hdupoll'] = '0'

        local hduChnVmpKey = hduChnKey .. '/vmp'
        if(redis.call('exists', hduChnVmpKey) == 1) then
            hduChnAttr['hduselvmp'] = '1'
            hduChnAttr['vmpstyle'] = RedisHget(hduChnVmpKey, 'vmpstyle')
            hduChnAttr['mode'] = RedisHget(hduChnVmpKey, 'mode')
            hduChnAttr['vmprim'] = RedisHget(hduChnVmpKey, 'vmprim')
            hduChnAttr['vmpaddalias'] = RedisHget(hduChnVmpKey, 'vmpaddalias')
        else 
            hduChnAttr['hduselvmp'] = '0'
        end

        local subChnNum = 0
        if(hduChnAttr['hdustyle'] == '0') then
            subChnNum = 1
        else
            subChnNum = 4
        end
        local hduMember = {}
        for subChnIndex = 1, subChnNum do
            local subChnKey =  hduChnKey .. '/' .. tostring(subChnIndex - 1)
            if(redis.call('exists', subChnKey) == 1) then
                local hduMemberAttr = {}
                hduMemberAttr['vmptype'] = RedisHget(subChnKey, 'membertype')
                hduMemberAttr['subchnidx'] = RedisHget(subChnKey, 'subchnidx')
                local mtIdentify = {}
                mtIdentify['mtid'] = RedisHget(subChnKey, 'mtid')
                mtIdentify['mcuidx'] = RedisHget(subChnKey, 'mtcascade')
                hduMemberAttr['mtidentify'] = mtIdentify

                hduMember[#hduMember + 1] = hduMemberAttr
            end
        end
        hduChnAttr['hdumember'] = hduMember
        hduChnArry[#hduChnArry + 1] = hduChnAttr
    end
end
restoreReq['hduparam'] = hduChnArry

--hdu batchpoll
local hduBatchPoll = {}
local hduBatchPollKey = confKey .. '/tvwall/batchpoll'
if(redis.call('exists', hduBatchPollKey) == 1) then
    hduBatchPoll['hduschedidx'] = RedisHget(hduBatchPollKey, 'schedidx')
    hduBatchPoll['pollnum'] = RedisHget(hduBatchPollKey, 'pollnum')
    hduBatchPoll['keeptime'] = RedisHget(hduBatchPollKey, 'keeptime')

    local hduNum = RedisHget(hduBatchPollKey, 'hdunum')
    local hduBatchPollChnArry = {}
    for hduIndex = 1, hduNum do
        local hduBatchPollChannelKey = hduBatchPollKey .. '/hdu/' .. (hduIndex - 1)
        local hduChnInfo = {}
        hduChnInfo['hduid'] = RedisHget(hduBatchPollChannelKey, 'hduid')
        hduChnInfo['chnnlidx'] = RedisHget(hduBatchPollChannelKey, 'chnidx')

        hduBatchPollChnArry[#hduBatchPollChnArry + 1] = hduChnInfo
    end
    hduBatchPoll['hduchninfo'] = hduBatchPollChnArry
end
restoreReq['batchpoll'] = hduBatchPoll

------------------------------------------------------------
local autoRecAttrb = {}
local recs = redis.call('smembers', confKey .. '/recs')
for recIndex, recID in ipairs(recs) do
    local recKey = confKey .. '/rec/' .. recID
    local recAttr = {}
    recAttr['recname'] = RedisHget(recKey, 'name')
    recAttr['publishmode'] = RedisHget(recKey, 'publishmode')
    recAttr['isrecmainvideo'] = RedisHget(recKey, 'recmainvideo')
    recAttr['isrecdstream'] = RedisHget(recKey, 'recdstream')
    recAttr['ismerge'] = RedisHget(recKey, 'ismerge')
    recAttr['recmode'] = RedisHget(recKey, 'recmode')
    recAttr['isanonymous'] = RedisHget(recKey, 'isanonymous')

    local vrsAlias = {}
    local recAddr = RedisHget(recKey, 'addr')
    local recE164 = RedisHget(recKey, 'e164')
    vrsAlias['addr'] = recAddr
    if(recE164 ~= '') then
        vrsAlias['aliastype'] = '2'
    elseif(recAddr ~= '') then
        vrsAlias['aliastype'] = '1'
    else
        vrsAlias['aliastype'] = '0'
    end
    vrsAlias['vrsNO'] = RedisHget(recKey, 'alias')

    local mtID = RedisHget(recKey, 'mtid')
    local mtArry = {}
    if(mtID ~= '0') then
        local mtIdentify = {}
        mtIdentify['mtid'] = mtID
        mtIdentify['mcuidx'] = RedisHget(recKey, 'mtcascade')
        local mtItem = {}
        mtItem['mtidentify'] = mtIdentify
        mtItem['mtchnid'] = '1'
        mtArry[#mtArry + 1] = mtItem
    end
    vrsAlias['mt'] = mtArry

    recAttr['vrsalias'] = vrsAlias
    autoRecAttrb[#autoRecAttrb + 1] = recAttr
end
restoreReq['autorecattrb'] = autoRecAttrb

------------------------------------------------------------
local playKey = confKey .. '/play'
if(redis.call('exists', playKey) == 1) then
    local playAttr = {}
    playAttr['filename'] = RedisHget(playKey, 'name')
    playAttr['fileid'] = RedisHget(playKey, 'fileid')
    playAttr['curprog'] = RedisHget(playKey, 'curprog')

    local vrsAlias = {}
    local vrsAddr = RedisHget(playKey, 'addr')
    local vrsE164 = RedisHget(playKey, 'e164')
    vrsAlias['addr'] = vrsAddr
    if(vrsE164 ~= '') then
        vrsAlias['aliastype'] = '2'
    elseif(vrsAddr ~= '') then
        vrsAlias['aliastype'] = '1'
    else
        vrsAlias['aliastype'] = '0'
    end
    vrsAlias['vrsNO'] = RedisHget(playKey, 'alias')

    playAttr['vrsalias'] = vrsAlias
    restoreReq['playattrb'] = playAttr
end

------------------------------------------------------------
local reqinfo = {}
restoreReq['reqheadinfo'] = reqinfo
------------------------------------------------------------
local restoreReqJson = cjson.encode(restoreReq)
return restoreReqJson
