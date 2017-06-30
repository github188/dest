
local cmuIp = ARGV[1]
local cmu_ip_confs_key = "cmu/" .. cmuIp .. "/confs"
local cmuConfNum = redis.call('scard', cmu_ip_confs_key)
return cmuConfNum
