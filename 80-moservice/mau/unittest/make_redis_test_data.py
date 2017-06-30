import redis

r = redis.Redis('localhost',6379, 0)
r.flushdb()

################global##############################################
##license
r.hset('domain/d2yfswr73u9o4ragiygzbwn5/license', 'maxlicensedconfnum', '64')
r.hset('domain/d2yfswr73u9o4ragiygzbwn5/license', 'licensedconfnum', '2')
r.hset('domain/d2yfswr73u9o4ragiygzbwn5/license', 'unlicensedconfnum', '0')
##mcu
r.sadd('cmus', '172.16.186.31')

################case_1###############################################
firstConfE164 = '1118998'
r.sadd('confs', firstConfE164)
r.sadd('cmu/172.16.186.31/confs', firstConfE164)
r.hset('domain/d2yfswr73u9o4ragiygzbwn5/confs',firstConfE164,'2')

###############conf/e164##################################
confdetail={
    'e164':firstConfE164,
    'moid':'d2yfswr73u9o4ragiygzbwn5',
    'creatorname':'0512110000120',
    'creatormoid':'86c08bda-5f42-465f-aec3-20a9e0aae4bf',
    'confname':'reidsTest',
    'starttime':'2016-01-18 11:19:28',
    'endtime':'2016-01-18 15:19:28',
    'conftype':'0',
    'password':'',
    'duration':'240',
    'encryptmode':'0',
    'encryptkey':'',
    'vacinterval':'0',
    'videoprior':'1',
    'allmute':'0',
    'dualmode':'0',
    'speakersrc':'0',
    'callmode':'0',
    'callinterval':'10',
    'calltimes':'3',
    'publicconf':'0',
    'openmode':'1',
    'nodisturb':'0',
    'rentalmode':'0',
    'datamode':'0',
    'dcast':'0',
    'dcastip':'',
    'dcastprot':'',
    'cascademode':'1',
    'isupload':'1',
    'isspy':'1',
    'spybandwidth':'8192',
    'mediatype':'106',
    'mediabitrate':'2048',
    'mediares':'5',
    'mediaframe':'188',
    'forcebrd':'0',
    'maxjoinedmt':'192'
}
r.hmset('conf/' + firstConfE164,confdetail)

############conf/e164/monitor#########################
r.sadd('conf/' + firstConfE164 + '/monitors', "172.16.11.11/10001")
r.sadd('conf/' + firstConfE164 + '/monitors', "172.16.11.11/10002")
r.sadd('conf/' + firstConfE164 + '/monitors', "172.16.11.12/10001")
r.sadd('conf/' + firstConfE164 + '/monitors', "172.16.11.12/10002")

monitorinfo = {
        'ip':'172.16.11.11',
        'port':'10001',
        'mediamode':'1',
        'alivetime':''
}
r.hmset('conf/' + firstConfE164 + '/monitor/172.16.11.11/10001', monitorinfo)
monitorinfo['port'] = '10002'
monitorinfo['mediamode'] = '2'
r.hmset('conf/' + firstConfE164 + '/monitor/172.16.11.11/10002',monitorinfo)
monitorinfo['ip'] = '172.16.11.12'
r.hmset('conf/' + firstConfE164 + '/monitor/172.16.11.12/10002',monitorinfo)
monitorinfo['port'] = '10001'
monitorinfo['mediamode'] = '1'
r.hmset('conf/' + firstConfE164 + '/monitor/172.16.11.12/10001',monitorinfo)

###########conf/e164/mt###########################
r.sadd('conf/' + firstConfE164 + '/mts', '1')
r.sadd('conf/' + firstConfE164 + '/mts', '2');
r.sadd('conf/' + firstConfE164 + '/mts', '3');

mtid = {
    'e164':'0512110200112',
    'mtid':'1',
    'mtalias':'gkmt_112',
    'mttype':'3',
    'mtip':'172.16.186.31',
    'online':'1',
    'bitrate':'256',
    'silence':'0',
    'mute':'0',
    'dual':'0',
    'mix':'0',
    'poll':'0',
    'tvwall':'0',
    'vmp':'0',
    'select':'0',
    'video':'0'
}
r.hmset('conf/' + firstConfE164 + '/mt/' + '1', mtid)

mtid['e164'] = '0512110200113'
mtid['mtid'] = '2'
mtid['mtalias'] = 'gkmt_113'
r.hmset('conf/' + firstConfE164 + '/mt/' + '2', mtid)

mtid['e164'] = '0512110200114'
mtid['mtid'] = '3'
mtid['mtalias'] = 'gkmt_114'
r.hmset('conf/' + firstConfE164 + '/mt/' + '3', mtid)

###########conf/e164/vmp############################
r.sadd('conf/' + firstConfE164 + '/vmps', 1)

vmpid = {
    'vmpbrdst':'1',
    'vmpstyle':'2',
    'chnnum':'2',
    'mode':'',
    'batchpolltime':'30',
    'batchpollnum':'30',
    'vmprim':'0',
    'vmpaddalias':'1',
    'voicehint':'1'
}
r.hmset('conf/' + firstConfE164 + '/vmp/1', vmpid)

vmpchannel = {
    'chnidx':'0',
    'vmptype':'0',
    'vmpstatus':'4',
    'mttype':'3',
    'mtid':'1',
    'mtcascade':'',
    'mtchnidx':'0'
}
r.hmset('conf/' + firstConfE164 + '/vmp/1/channel/0', vmpchannel)
vmpchannel['chnidx'] = '1'
vmpchannel['mtid'] = '2'
r.hmset('conf/' + firstConfE164 + '/vmp/1/channel/1', vmpchannel)

vmpchannelpoll = {
    'pollstate':'1',
    'pollnum':'30',
    'keeptime':'30',
    'curidx':'0',
    'mtnum':'1'
}
r.hmset('conf/' + firstConfE164 + '/vmp/1/channel/0/poll', vmpchannelpoll)
r.hmset('conf/' + firstConfE164 + '/vmp/1/channel/1/poll', vmpchannelpoll)

vmpchannelpollmember = {
    'mtid':'1',
    'mtcascade':'',
    'mtchnidx':'0'
}
r.hmset('conf/' + firstConfE164 + '/vmp/1/channel/0/poll/member/0', vmpchannelpollmember)
vmpchannelpollmember['mtid'] = '2'
r.hmset('conf/' + firstConfE164 + '/vmp/1/channel/1/poll/member/0', vmpchannelpollmember)

##########conf/e164/mix#############################
r.sadd('conf/' + firstConfE164 + '/mixs', 1)

mix = {
    'mixmode':'1',
    'vacinterval':'',
    'mtnum':'2'
}
r.hmset('conf/' + firstConfE164 + '/mix/1', mix)

mixmember = {
    'mtid':'1',
    'mtcascade':'',
    'mttype':'3',
    'mtchnidx':'0'
}
r.hmset('conf/' + firstConfE164 + '/mix/1/member/1', mixmember);

mixmember['mtid'] = '2'
mixmember['mtchnidx'] = '1'
r.hmset('conf/' + firstConfE164 + '/mix/1/member/2', mixmember);

###########################################################################################
#############case_2#####################################
secondConfE164 = '1118997'
r.sadd('confs', secondConfE164)
r.sadd('cmu/172.16.186.31/confs', secondConfE164)
r.hset('domain/d2yfswr73u9o4ragiygzbwn5/confs', secondConfE164,'2')

##################conf/e164############################
confdetail['e164']=secondConfE164
r.hmset('conf/' + secondConfE164,confdetail)

#################conf/e164/monitor####################
r.sadd('conf/' + secondConfE164 + '/monitors', "172.16.11.13/10001")
r.sadd('conf/' + secondConfE164 + '/monitors', "172.16.11.13/10002")
r.sadd('conf/' + secondConfE164 + '/monitors', "172.16.11.14/10001")
r.sadd('conf/' + secondConfE164 + '/monitors', "172.16.11.14/10002")

monitorinfo['ip'] = '172.16.11.13'
monitorinfo['port'] = '10001'
monitorinfo['mediamode'] = '1'
r.hmset('conf/' + secondConfE164 + '/monitor/172.16.11.13/10001', monitorinfo)
monitorinfo['port'] = '10002'
monitorinfo['mediamode'] = '2'
r.hmset('conf/' + secondConfE164 + '/monitor/172.16.11.13/10002',monitorinfo)
monitorinfo['ip'] = '172.16.11.14'
r.hmset('conf/' + secondConfE164 + '/monitor/172.16.11.14/10002',monitorinfo)
monitorinfo['port'] = '10001'
monitorinfo['mediamode'] = '1'
r.hmset('conf/' + secondConfE164 + '/monitor/172.16.11.14/10001',monitorinfo)

#################conf/e164/mt########################
r.sadd('conf/' + secondConfE164 + '/mts', '1')
r.sadd('conf/' + secondConfE164 + '/mts', '2');
r.sadd('conf/' + secondConfE164 + '/mts', '3');

mtid['e164'] = '0512110200112'
mtid['mtalias'] = 'gkmt_112'
r.hmset('conf/' + secondConfE164 + '/mt/' + '1', mtid)

mtid['e164'] = '0512110200113'
mtid['mtalias'] = 'gkmt_113'
r.hmset('conf/' + secondConfE164 + '/mt/' + '2', mtid)

mtid['e164'] = '0512110200115'
mtid['mtalias'] = 'gkmt_115'
r.hmset('conf/' + secondConfE164 + '/mt/' + '3', mtid)

###########conf/e164/vmp############################
r.sadd('conf/' + secondConfE164 + '/vmps', 1)

r.hmset('conf/' + secondConfE164 + '/vmp/1', vmpid)

vmpchannel['chnidx'] = '0'
vmpchannel['mtid'] = '1'
r.hmset('conf/' + secondConfE164 + '/vmp/1/channel/0', vmpchannel)
vmpchannel['chnidx'] = '1'
vmpchannel['mtid'] = '2'
r.hmset('conf/' + secondConfE164 + '/vmp/1/channel/1', vmpchannel)

r.hmset('conf/' + secondConfE164 + '/vmp/1/channel/0/poll', vmpchannelpoll)
r.hmset('conf/' + secondConfE164 + '/vmp/1/channel/1/poll', vmpchannelpoll)

vmpchannelpollmember['mtid'] = '1'
r.hmset('conf/' + secondConfE164 + '/vmp/1/channel/0/poll/member/0', vmpchannelpollmember)
vmpchannelpollmember['mtid'] = '2'
r.hmset('conf/' + secondConfE164 + '/vmp/1/channel/1/poll/member/0', vmpchannelpollmember)

##########conf/e164/mix#############################
r.sadd('conf/' + secondConfE164 + '/mixs', 1)

r.hmset('conf/' + secondConfE164 + '/mix/1', mix)

mixmember['mtid'] = '1'
mixmember['mtchnidx'] = '0'
r.hmset('conf/' + secondConfE164 + '/mix/1/member/1', mixmember);

mixmember['mtid'] = '2'
mixmember['mtchnidx'] = '1'
r.hmset('conf/' + secondConfE164 + '/mix/1/member/2', mixmember);
