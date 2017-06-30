
local confE164 = ARGV[1]

local monitorList = {}
local confMonitorIpAndPortList = redis.call('smembers', 'conf/' .. confE164 .. '/monitors')
for monitorIndex = 1, #confMonitorIpAndPortList do
    local conf_e164_monitor_ipAndPort_key = 'conf/' .. confE164 .. '/monitor/' .. confMonitorIpAndPortList[monitorIndex]
    local monitorInfo = redis.call('hgetall', conf_e164_monitor_ipAndPort_key)
    local monitorHashInfo = {}
    for index = 1, #monitorInfo, 2 do
        monitorHashInfo[monitorInfo[index]] = monitorInfo[index + 1]
    end
	monitorHashInfo['e164'] = confE164
    monitorList[monitorIndex] = monitorHashInfo
end

local monitorListJsonStr = cjson.encode(monitorList)
redis.log(redis.LOG_WARNING, "conf monitos: " .. monitorListJsonStr)
return monitorListJsonStr

