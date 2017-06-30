
local domainMoid = ARGV[1]
local maxDomainSmallConfNum = ARGV[2]
local maxDomainLargeConfNum = ARGV[3]
local domain_moid_license_key = "domain/" .. domainMoid .. "/license"
redis.call('hset', domain_moid_license_key, 'maxlicensedsmallconfnum',
 maxDomainSmallConfNum)
redis.call('hset', domain_moid_license_key, 'maxlicensedlargeconfnum',
 maxDomainLargeConfNum)

