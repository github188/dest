import json
from .generalhandle import GeneralHandle
from .generalhandle import BaseHandle
from .converter import api2mc
from .converter import mc2api
import tornado.httpclient
import logging

logger = logging.getLogger('API')

APIVersion = 'V5.1.0.1.0.20170421'

CM_MAU_MQ = {
    'ex': 'mau.cmmau.ex',
    'key': 'mau.cmmau.k',
}

CM_MCU_MQ = {
    'ex': 'mau.cmmcu.ex',
    'key': 'mau.cmmcu.k',
}


class ConfListHandle(GeneralHandle):
    """
    GET /confs
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getconflist.lua',
        'converter': api2mc.conflist,
    }

class MtHandle(GeneralHandle):
    """
    GET /conf/{conf_id}/mt/{mt_id}
    DELETE  /conf/{conf_id}/mt/{mt_id}
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getmtinfo.lua',
        'converter': api2mc.getmtinfo,
    }

    _delete = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
          'msg': 'CM_CMU_DELMT_REQ',
          'converter': api2mc.delmt,
        },
        'ack': {
            'msg': 'CMU_CM_DELMT_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_DELMT_NACK',
            'converter': mc2api.defaultnack,
        },
    }


class CallMtHandle(GeneralHandle):
    """
    PUT /conf/{conf_id}/mt/{mt_id}/online
    """
    _put = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CALLMT_REQ',
            'converter': api2mc.callmt,
        },
        'ack': {
            'msg': 'CMU_CM_CALLMT_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CALLMT_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _delete = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_DROPMT_REQ',
            'converter': api2mc.callmt,
        },
        'ack': {
            'msg': 'CMU_CM_DROPMT_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_DROPMT_NACK',
            'converter': mc2api.defaultnack,
        },
    }

class CancelInspectMtHandle(GeneralHandle):
    """
    DELETE confs/{conf_id}/inspections/{mt_id}/{mode}
    """
    _delete_mt = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPSEEMT_REQ',
            'converter': api2mc.newstopinspectmt,
        },
        'ack': {
            'msg': 'CMU_CM_STOPSEEMT_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPSEEMT_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _delete_vmp = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPSEEVMP_REQ',
            'converter': api2mc.newinspectvmp,
        },
        'ack': {
            'msg': 'CMU_CM_STOPSEEVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPSEEVMP_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def delete(self, *args, **kw):
        key = 'conf/%s/selected/video/%s/1' % (self.path_args[0], self.path_args[1])
        inspectinfo = self.db.hgetall(key)
        if inspectinfo and inspectinfo[b'srctype'].decode() == '2':
            await self.handle(self._delete_vmp, *args)
        else:
            await self.handle(self._delete_mt, *args)

class InspectMtHandle(GeneralHandle):
    """
    GET  confs/{conf_id}/inspections
    POST confs/{conf_id}/inspections
    DELETE confs/{conf_id}/inspections
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getinspectionlist.lua',
        'converter': api2mc.getinspectionlist,
    }

    _post_mt = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTSEEMT_REQ',
            'converter': api2mc.inspectmt,
        },
        'ack': {
            'msg': 'CMU_CM_STARTSEEMT_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTSEEMT_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _post_vmp = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTSEEVMP_REQ',
            'converter': api2mc.inspectvmp,
        },
        'ack': {
            'msg': 'CMU_CM_STARTSEEVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTSEEVMP_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _delete_mt = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPSEEMT_REQ',
            'converter': api2mc.stopinspectmt,
        },
        'ack': {
            'msg': 'CMU_CM_STOPSEEMT_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPSEEMT_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _delete_vmp = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPSEEVMP_REQ',
            'converter': api2mc.inspectvmp,
        },
        'ack': {
            'msg': 'CMU_CM_STOPSEEVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPSEEVMP_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def post(self, *args, **kw):
        data = self.get_argument('params')
        reqobj = json.loads(data)
        if reqobj['src']['type'] == 2:
            await self.handle(self._post_vmp, *args)
        else:
            await self.handle(self._post_mt, *args)

    async def delete(self, *args, **kw):
        data = self.get_argument('params')
        reqobj = json.loads(data)
        key = 'conf/%s/selected/video/%s/1' % (args[0], reqobj['dst']['mt_id'])
        inspectinfo = self.db.hgetall(key)
        if inspectinfo and inspectinfo[b'srctype'].decode() == '2':
            await self.handle(self._delete_vmp, *args)
        else:
            await self.handle(self._delete_mt, *args)

class ChairManHandle(GeneralHandle):
    """
    GET confs/{conf_id}/chairman
    PUT confs/{conf_id}/chairman
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getchairman.lua',
        'converter': api2mc.getchairman,
    }

    _put_set = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_SETCHAIRMAN_REQ',
            'converter': api2mc.setchairman,
        },
        'ack': {
            'msg': 'CMU_CM_SETCHAIRMAN_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_SETCHAIRMAN_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _put_cancel = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CANCELCHAIRMAN_REQ',
            'converter': api2mc.setchairman,
        },
        'ack': {
            'msg': 'CMU_CM_CANCELCHAIRMAN_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CANCELCHAIRMAN_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def put(self, *args, **kw):
        data = self.get_argument('params')
        reqobj = json.loads(data)
        if reqobj['mt_id'] != '':
            await self.handle(self._put_set, *args)
        else:
            await self.handle(self._put_cancel, *args)

class UploadMtHandle(GeneralHandle):
    """
    GET confs/{conf_id}/upload
    PUT confs/{conf_id}/upload
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getupload.lua',
        'converter': api2mc.getupload,
    }

    _put_start = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_SPECSRCOFMMCU_REQ',
            'converter': api2mc.startupload,
        },
        'ack': {
            'msg': 'CMU_CM_SPECSRCOFMMCU_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_SPECSRCOFMMCU_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _put_cancel = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_CANCELSRCOFMMCU_CMD',
            'converter': api2mc.sete164,
        }
    }

    async def put(self, *args, **kw):
        data = self.get_argument('params')
        reqobj = json.loads(data)
        if reqobj['mt_id'] != '':
            await self.handle(self._put_start, *args)
        else:
            await self.handle(self._put_cancel, *args)


class SpeakerHandle(GeneralHandle):
    """
    GET conf/{conf_id}/speaker
    PUT conf/{conf_id}/speaker
    """

    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getspeaker.lua',
        'converter': api2mc.getspeaker,
    }

    _put_set = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_SETSPEAKER_REQ',
            'converter': api2mc.setspeaker,
        },
        'ack': {
            'msg': 'CMU_CM_SETSPEAKER_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_SETSPEAKER_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _put_cancel = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CANCELSPEAKER_REQ',
            'converter': api2mc.sete164,
        },
        'ack': {
            'msg': 'CMU_CM_CANCELSPEAKER_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CANCELSPEAKER_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['mt_id'] != '':
            await self.handle(self._put_set, *args)
        else:
            await self.handle(self._put_cancel, *args)


class ConfSilenceHandle(GeneralHandle):
    """
    PUT conf/{conf_id}/silence
    """
    _put_set = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CONFSILENCE_REQ',
            'converter': api2mc.sete164,
        },
        'ack': {
            'msg': 'CMU_CM_CONFSILENCE_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CONFSILENCE_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _put_cancel = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CANCELCONFSILENCE_REQ',
            'converter': api2mc.sete164,
        },
        'ack': {
            'msg': 'CMU_CM_CANCELCONFSILENCE_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CANCELCONFSILENCE_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['value'] == 1:
            await self.handle(self._put_set, *args)
        else:
            await self.handle(self._put_cancel, *args)


class ConfMuteHandle(GeneralHandle):
    """
    PUT conf/{conf_id}/mute
    """
    _put_set = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CONFMUTE_REQ',
            'converter': api2mc.sete164,
        },
        'ack': {
            'msg': 'CMU_CM_CONFMUTE_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CONFMUTE_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _put_cancel = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CANCELCONFMUTE_REQ',
            'converter': api2mc.sete164,
        },
        'ack': {
            'msg': 'CMU_CM_CANCELCONFMUTE_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CANCELCONFMUTE_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['value'] == 1:
            await self.handle(self._put_set, *args)
        else:
            await self.handle(self._put_cancel, *args)


class ConfHandle(GeneralHandle):
    """
    GET conf/{conf_id}
    """
    _get = {
        'action': 'redis',
        'converter': api2mc.conf,
        'lua': 'v1/lua/getconf.lua'
    }

class ConfState(GeneralHandle):
    """
    GET confs/{conf_id}/state
    """
    _get = {
        'action': 'redis',
        'converter': api2mc.confstate,
        'lua': 'v1/lua/getconfstate.lua'
    }


class CascadesConfHandle(GeneralHandle):
    """
    GET conf/{conf_id}/cascades
    """
    _get = {
        'action': 'redis',
        'converter': api2mc.getcascadeconf,
        'lua': 'v1/lua/getcascades.lua'
    }

class CascadesMtHandle(GeneralHandle):
    """
    GET conf/{conf_id}/cascades/{cascade_id}/mts
    POST confs/{conf_id}/cascades/{cascade_id}/mts
    """
    _get = {
        'action': 'redis',
        'converter': api2mc.getcascademts,
        'lua': 'v1/lua/getcascademts.lua'
    }

    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_ADDMT_REQ',
            'converter': api2mc.addcascadesmt,
        },
        'ack': {
            'msg': 'CMU_CM_ADDMT_ACK',
            'converter': mc2api.cascadesaddmtack,
        },
        'nack': {
            'msg': 'CMU_CM_ADDMT_NACK',
            'converter': mc2api.defaultnack,
        },
    }


class DelayConfHandle(GeneralHandle):
    """
    POST conf/{conf_id}/delay
    """
    _post = {
        'action': 'mqrpc',
        'mq': CM_MAU_MQ,
        'req': {
            'msg': 'CM_MAU_DELAYCONF_REQ',
            'converter': api2mc.delayconf,
        },
        'ack': {
            'msg': 'MAU_CM_DELAYCONF_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'MAU_CM_DELAYCONF_NACK',
            'converter': mc2api.defaultnack,
        },
    }


class MtSlienceHandle(GeneralHandle):
    """
    PUT conf/{conf_id}/mt/{mt_id}/silence
    """
    _put_set = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_MTSILENCE_REQ',
            'converter': api2mc.setmtchnl,
        },
        'ack': {
            'msg': 'CMU_CM_MTSILENCE_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_MTSILENCE_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _put_cancel = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CANCELMTSILENCE_REQ',
            'converter': api2mc.setmtchnl,
        },
        'ack': {
            'msg': 'CMU_CM_CANCELMTSILENCE_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CANCELMTSILENCE_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['value'] == 1:
            await self.handle(self._put_set, *args)
        else:
            await self.handle(self._put_cancel, *args)


class MtMuteHandle(GeneralHandle):
    """
    PUT conf/{conf_id}/mt/{mt_id}/mute
    """
    _put_set = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_MTMUTE_REQ',
            'converter': api2mc.setmtchnl,
        },
        'ack': {
            'msg': 'CMU_CM_MTMUTE_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_MTMUTE_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _put_cancel = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CANCELMTMUTE_REQ',
            'converter': api2mc.setmtchnl,
        },
        'ack': {
            'msg': 'CMU_CM_CANCELMTMUTE_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CANCELMTMUTE_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['value'] == 1:
            await self.handle(self._put_set, *args)
        else:
            await self.handle(self._put_cancel, *args)


class ConfMonitorHandle(GeneralHandle):
    """
    GET conf/{conf_id}/monitors
    POST conf/{conf_id}/monitors
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getmonitorlist.lua',
        'converter': api2mc.getmonitorlist,
    }

    _post_start = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTMONITORING_REQ',
            'converter': api2mc.startmonitor,
        },
        'ack': {
            'msg': 'CMU_CM_STARTMONITORING_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTMONITORING_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _post_startvmp = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTMONITORING_VMP_REQ',
            'converter': api2mc.startmonitorvmp,
        },
        'ack': {
            'msg': 'CMU_CM_STARTMONITORING_VMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTMONITORING_VMP_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _post_startmix = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTMONITORING_MIXER_REQ',
            'converter': api2mc.startmonitormix,
        },
        'ack': {
            'msg': 'CMU_CM_STARTMONITORING_MIXER_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTMONITORING_MIXER_NACK',
            'converter': mc2api.defaultnack,
        },
    }

#    _delete = {
#        'action': 'mqpush',
#        'mq': CM_MCU_MQ,
#        'cmd': {
#            'msg': 'CM_CMU_STOPMONITORING_CMD',
#            'converter': api2mc.stopmonitor,
#        },
#    }

    async def post(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['mode'] == 0 and reqobj['src']['type'] == 2:
            await self.handle(self._post_startvmp, *args)
        elif reqobj['mode'] == 1 and reqobj['src']['type'] == 3:
            await self.handle(self._post_startmix, *args)
        else:
            await self.handle(self._post_start, *args)

class ConfSigleMonitorHandle(GeneralHandle):
    """
    GET  confs/{conf_id}/monitors/{dst_ip}/{dst_port}
    DELETE confs/{conf_id}/monitors/{dst_ip}/{dst_port}
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getmonitorinfo.lua',
        'converter': api2mc.getmonitorinfo,
    }

    _delete = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STOPMONITORING_CMD',
            'converter': api2mc.stopmonitor,
        },
    }

class NeedIFrameHandle(GeneralHandle):
    """
    POST  /api/v1/vc/confs/{conf_id}/neediframe/monitors
    """
    _post = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_NEEDIFRAME_CMD',
            'converter': api2mc.neediframe,
        }
    }

class MonitorAliveHandle(GeneralHandle):
    """
    POST conf/{conf_id}/monitor/keepalive
    """
    _post = {
        'action': 'mqpush',
        'mq': CM_MAU_MQ,
        'cmd': {
            'msg': 'CM_MAU_MONITORKEEPALIVE_NTF',
            'converter': api2mc.monitorkeepalive,
        }
    }

class ConfSaftyHandle(GeneralHandle):
    """
    POST conf/{conf_id}/safety
    """
    _put = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_SETCONFSAFE_REQ',
            'converter': api2mc.setconfsafty,
        },
        'ack': {
            'msg': 'CMU_CM_SETCONFSAFE_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_SETCONFSAFE_NACK',
            'converter': mc2api.defaultnack,
        },
    }

class VmpStartHandle(GeneralHandle):
    """
    POST conf/{conf_id}/vmp
    """
    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTVMP_REQ',
            'converter': api2mc.startvmp,
        },
        'ack': {
            'msg': 'CMU_CM_STARTVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTVMP_NACK',
            'converter': mc2api.defaultnack,
        },
    }


class VmpHandle(GeneralHandle):
    """
    GET  conf/{conf_id}/vmp/{vmp_id}
    POST conf/{conf_id}/vmp/{vmp_id}
    DELETE conf/{conf_id}/vmp/{vmp_id}
    """
    _get = {
        'action': 'redis',
        'converter': api2mc.getvmpinfo,
        'lua': 'v1/lua/getvmpinfo.lua'
    }

    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CHANGEVMPPARAM_REQ',
            'converter': api2mc.changevmp,
        },
        'ack': {
            'msg': 'CMU_CM_CHANGEVMPPARAM_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CHANGEVMPPARAM_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _delete = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPVMP_REQ',
            'converter': api2mc.stopvmp,
        },
        'ack': {
            'msg': 'CMU_CM_STOPVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPVMP_NACK',
            'converter': mc2api.defaultnack,
        },
    }


class StartMixerHandle(GeneralHandle):
    """
    POST conf/{conf_id}/mix
    """
    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTMIX_REQ',
            'converter': api2mc.startmixer,
        },
        'ack': {
            'msg': 'CMU_CM_STARTMIX_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTMIX_NACK',
            'converter': mc2api.defaultnack,
        },
    }


class MixerHandle(GeneralHandle):
    """
    GET  conf/{conf_id}/mix/{mix_id}
    DELETE conf/{conf_id}/mix/{mix_id}
    """

    _get = {
        'action': 'redis',
        'converter': api2mc.getmixer,
        'lua': 'v1/lua/getmixer.lua'
    }

    _delete = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPMIX_REQ',
            'converter': api2mc.stopmix,
        },
        'ack': {
            'msg': 'CMU_CM_STOPMIX_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPMIX_NACK',
            'converter': mc2api.defaultnack,
        },
    }


class MixerMembersHandle(GeneralHandle):
    """
    POST conf/{conf_id}/mix/{mix_id}/members
    DELETE conf/{conf_id}/mix/{mix_id}/members
    """
    _post = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_ADDMIXMEMBER_CMD',
            'converter': api2mc.setmixmembers,
        },
    }

    _delete = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_REMOVEMIXMEMBER_CMD',
            'converter': api2mc.setmixmembers,
        },
    }

class ConfPollStateHandle(GeneralHandle):
    """
    PUT conf/{conf_id}/poll/state
    """
    _put_pause = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_PAUSEPOLL_CMD',
            'converter': api2mc.handlepollstate,
        },
    }

    _put_resume = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_RESUMEPOLL_CMD',
            'converter': api2mc.handlepollstate,
        },
    }

    _put_stop = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STOPPOLL_CMD',
            'converter': api2mc.handlepollstate,
        },
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        funcdict = {
            0: self._put_stop,
            1: self._put_pause,
            2: self._put_resume,
        }
        await self.handle(funcdict[reqobj['value']], *args)

class ConfPollHandle(GeneralHandle):
    """
    GET conf/{conf_id}/poll
    PUT conf/{conf_id}/poll/
    """
    _get = {
        'action': 'redis',
        'converter': api2mc.getpollinfo,
        'lua': 'v1/lua/getpollinfo.lua'
    }

    _put = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STARTPOLL_CMD',
            'converter': api2mc.startpoll,
        },
    }

class ConfVadHandle(GeneralHandle):
    """
    GET /confs/([0-9]+)/vad
    PUT confs/([0-9]+)/vad
    """
    _get = {
        'action': 'redis',
        'converter': api2mc.getvadinfo,
        'lua': 'v1/lua/getvadinfo.lua'
    }
    _put_start = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STARTMIX_REQ',
            'converter': api2mc.startvad,
        },
        'ack': {
            'msg': 'CM_CMU_STARTMIX_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CM_CMU_STARTMIX_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _put_stop = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STOPMIX_REQ',
            'converter': api2mc.stopvad,
        },
        'ack': {
            'msg': 'CMU_CM_STOPMIX_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPMIX_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        funcdict = {
            0: self._put_stop,
            1: self._put_start,
        }
        await self.handle(funcdict[reqobj['state']], *args)

class ShortMsgHandle(GeneralHandle):
    """
    POST conf/{conf_id}/msg
    """
    _post = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_SENDMSG_CMD',
            'converter': api2mc.sendmsg,
        }
    }

class MtRecHandle(GeneralHandle):
    """
    POST confs/{conf_id}/mt_recoder/{mt_id}
    """
    _post_pause = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_PAUSEPLAY_REQ',
#            'converter': api2mc.rechandle,
        },
        'ack': {
            'msg': 'CMU_CM_PAUSEPLAY_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_PAUSEPLAY_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _post_resume = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_RESUMEPLAY_REQ',
  #          'converter': api2mc.rechandle,
        },
        'ack': {
            'msg': 'CMU_CM_RESUMEPLAY_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_RESUMEPLAY_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    _post_stop = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STOPPLAY_REQ',
 #           'converter': api2mc.rechandle,
        },
        'ack': {
            'msg': 'CMU_CM_STOPPLAY_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPPLAY_NACK',
            'converter': mc2api.defaultnack,
        },
    }

    async def post(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        funcdict = {
            0: self._post_pause,
            1: self._post_resume,
            2: self._post_stop,
        }
        await self.handle(funcdict[reqobj['state']], *args)


class MtListVmpsHandle(GeneralHandle):
    """
    GET confs/{conf_id}/mtvmps
    POST confs/{conf_id}/mtvmps
    """
    _get = {
        'action': 'redis',
        'converter': api2mc.getmtlistvmps,
        'lua': 'v1/lua/getmtlistvmps.lua'
    }

    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTMTVMP_REQ',
            'converter': api2mc.startmtvmp,
        },
        'ack': {
            'msg': 'CMU_CM_STARTMTVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTMTVMP_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class MtVmpsHandle(GeneralHandle):
    """
    GET confs/{conf_id}/mtvmps/{mt_id}
    PUT confs/{conf_id}/mtvmps/{mt_id}
    DELETE confs/{conf_id}/mtvmps/{mt_id}
    """
    _get = {
        'action': 'redis',
        'converter': api2mc.getmtvmps,
        'lua': 'v1/lua/getmtvmps.lua'
    }

    _put = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CHANGEMTVMPPARAM_REQ',
            'converter': api2mc.changemtvmps,
        },
        'ack': {
            'msg': 'CMU_CM_CHANGEMTVMPPARAM_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CHANGEMTVMPPARAM_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _delete = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPMTVMP_REQ',
            'converter': api2mc.stopmtvmps,
        },
        'ack': {
            'msg': 'CMU_CM_STOPMTVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPMTVMP_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class HduListVmpsHandle(GeneralHandle):
    """
    GET confs/{conf_id}/hduvmps
    POST confs/{conf_id}/hduvmps
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/gethduvmpslist.lua',
        'converter': api2mc.gethduvmpslist,
    }

    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTHDUVMP_REQ',
            'converter': api2mc.starthduvmps,
        },
        'ack': {
            'msg': 'CMU_CM_STARTHDUVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTHDUVMP_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class HduVmpsHandle(GeneralHandle):
    """
    GET confs/{conf_id}/hduvmps/{hdu_id}
    PUT confs/{conf_id}/hduvmps/{hdu_id}
    DELETE confs/{conf_id}/hduvmps/{hdu_id}
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/gethduvmps.lua',
        'converter': api2mc.gethduvmps,
    }

    _put = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CHANGEHDUVMP_REQ',
            'converter': api2mc.changehduvmps,
        },
        'ack': {
            'msg': 'CMU_CM_CHANGEHDUVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CHANGEHDUVMP_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _delete = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPHDUVMP_REQ',
            'converter': api2mc.stophduvmps,
        },
        'ack': {
            'msg': 'CMU_CM_STOPHDUVMP_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPHDUVMP_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class SpecDualkSrcHandle(GeneralHandle):
    """
    GET  confs/{conf_id}/dualstream
    PUT confs/{conf_id}/dualstream
    DELETE confs/{conf_id}/dualstream
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getdualstream.lua',
        'converter': api2mc.getdualstream,
    }

    _put_start = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_SPECDUALSRC_CMD',
            'converter': api2mc.specdualsrc,
        }
    }

    _put_stop = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_CANCELDUALSRC_CMD',
            'converter': api2mc.stopspecdualsrc,
        }
    }

    _delete = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_CANCELDUALSRC_CMD',
            'converter': api2mc.specdualsrc,
        }
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['mt_id'] != '':
            await self.handle(self._put_start, *args)
        else:
            key = "conf/%s/dualstream" % self.path_args[0]
            dualinfo = self.db.hgetall(key)
            if dualinfo:
                await self.handle(self._put_stop, *args, dualinfo[b'mtcascade'].decode(), dualinfo[b'mtid'].decode())
            else:
                msg = {
                    'success': 0,
                    'error_code': 22014,
                }
                self.write(msg)

class MtCameraRCHandle(GeneralHandle):
    """
    POST conf/{conf_id}/mt/{mt_id}/camera/state
    """
    _post_start = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_MTCAMERA_MOV_CMD',
            'converter': api2mc.camerarc,
        }
    }

    _post_stop = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_MTCAMERA_STOP_CMD',
            'converter': api2mc.camerarc,
        }
    }

    async def post(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['state'] == 0:
            await self.handle(self._post_start, *args)
        else:
            await self.handle(self._post_stop, *args)

class MtVolumHandle(GeneralHandle):
    """
    POST conf/{conf_id}/mt/{mt_id}/volum
    """
    _put = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_MODMTVOLUME_CMD',
            'converter': api2mc.mtvolum,
        }
    }

class ConfHduListHandle(GeneralHandle):
    """
    GET confs/{conf_id}/hdus
    POST confs/{conf_id}/hdus
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getconfhdulist.lua',
        'converter': api2mc.getconfhdulist,
    }

    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTHDUSWITCH_REQ',
            'converter': api2mc.newstarthdu,
        },
        'ack': {
            'msg': 'CMU_CM_STARTHDUSWITCH_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTHDUSWITCH_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _post_startpoll = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STARTHDUPOLL_CMD',
            'converter': api2mc.newstarthdupoll,
        },
    }

    _post_change = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_CHANGEHDUVMPMODE_CMD',
            'converter': api2mc.newstophdu,
        },
    }

    async def post(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['mode'] == 2:
            await self.handle(self._post_change, *args)
            self.clear()
        if reqobj['mode'] != 3:
            funcdict = self._post
        elif reqobj['mode'] == 3:
            funcdict = self._post_startpoll
        await self.handle(funcdict, *args)

class PlatformHduListHandle(GeneralHandle):
    """
    GET hdus
    """
    _get = {
        'action': 'mqrpc',
        'mq': CM_MAU_MQ,
        'req': {
            'msg': 'CM_MAU_GETHDUINFOBYMOID_REQ',
            'converter': api2mc.hdulist,
        },
        'ack': {
            'msg': 'MAU_CM_GETHDUINFOBYMOID_ACK',
            'converter': mc2api.hdulist,
        },
        'nack': {
            'msg': 'MAU_CM_GETHDUINFOBYMOID_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class HduHandle(GeneralHandle):
    """
    GET  conf/{conf_id}/hdu/{hdu_id}
    POST conf/{conf_id}/hdu/{hdu_id}
    DELETE conf/{conf_id}/hdu/{hdu_id}
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/gethduchninfo.lua',
        'converter': api2mc.gethduchninfo,
    }

    _post_start = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTHDUSWITCH_REQ',
            'converter': api2mc.starthdu,
        },
        'ack': {
            'msg': 'CMU_CM_STARTHDUSWITCH_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTHDUSWITCH_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _post_stop = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPHDUSWITCH_REQ',
            'converter': api2mc.stophdu,
        },
        'ack': {
            'msg': 'CMU_CM_STOPHDUSWITCH_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPHDUSWITCH_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _post_startpoll = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STARTHDUPOLL_CMD',
            'converter': api2mc.starthdupoll,
        },
    }

    _post_stoppoll = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STOPHDUPOLL_CMD',
            'converter': api2mc.stophdupoll,
        },
    }

    _delete_hdu = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPHDUSWITCH_REQ',
            'converter': api2mc.deletehdu,
        },
        'ack': {
            'msg': 'CMU_CM_STOPHDUSWITCH_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPHDUSWITCH_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _delete_poll = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_STOPHDUPOLL_CMD',
            'converter': api2mc.stophdupoll,
        },
    }

    _post_change = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_CHANGEHDUVMPMODE_CMD',
            'converter': api2mc.stophdu,
        },
    }

    async def delete(self, *args, **kw):
        hdu_id = self.path_args[1].replace('_', '/')
        key = 'conf/%s/tvwall/%s' % (self.path_args[0], hdu_id)
        hduinfo = self.db.hgetall(key)
        if hduinfo:
            if hduinfo[b'style'].decode() == '0' and hduinfo[b'hdutype'].decode() == '1':
                await self.handle(self._delete_poll, *args)
            else:
                await self.handle(self._delete_hdu, *args, hduinfo[b'style'].decode())
        else:
            msg = {
                'success': 0,
                'error_code': 20052,
            }
            self.write(msg)

    async def post(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['occupy'] == 1 and reqobj['mode'] == 2:
            await self.handle(self._post_change, *args)
            self.clear()
        if reqobj['mode'] != 3:
            funcdict = {
                0: self._post_stop,
                1: self._post_start,
            }
        elif reqobj['mode'] == 3:
            funcdict = {
                0: self._post_stoppoll,
                1: self._post_startpoll,
            }
        await self.handle(funcdict[reqobj['occupy']], *args)

class MtListHandle(GeneralHandle):
    """
    POST conf/{conf_id}/mtlist
    DELETE conf/{conf_id}/mtlist
    GET /conf/{conf_id}/mts'
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getconfmtlist.lua',
        'converter': api2mc.getconfmtlist,
    }

    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_ADDMT_REQ',
            'converter': api2mc.addmts,
        },
        'ack': {
            'msg': 'CMU_CM_ADDMT_ACK',
            'converter': mc2api.addmtack,
        },
        'nack': {
            'msg': 'CMU_CM_ADDMT_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _delete = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_DELMT_REQ',
            'converter': api2mc.delmts,
        },
        'ack': {
            'msg': 'CMU_CM_DELMT_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_DELMT_NACK',
            'converter': mc2api.defaultnack,
        }
    }


class MtsStateHandle(GeneralHandle):
    """
    POST conf/{conf_id}/mts/state
    """
    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_CALLMT_REQ',
            'converter': api2mc.setmts,
        },
        'ack': {
            'msg': 'CMU_CM_CALLMT_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_CALLMT_NACK',
            'converter': mc2api.defaultnack,
        }
    }
    _delete = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_DROPMT_REQ',
            'converter': api2mc.setmts,
        },
        'ack': {
            'msg': 'CMU_CM_DROPMT_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_DROPMT_NACK',
            'converter': mc2api.defaultnack,
        }
    }

async def recorderHandle(self, method_action, *args, **kw):
    try:
        account_info = await self.valid_token()
        if account_info is not None:
            if method_action is not None:
                method = self.request.method
                if method == 'POST':
                    params = self.get_argument('params')
                    reqobj = json.loads(params)
                if method == 'GET' or account_info['enableVRS'] and \
                        (reqobj['recorder_mode'] == 1 or account_info['enableLive']):
                    action = getattr(self, method_action['action'])
                    await action(method_action, account_info, *args, **kw)
                else:
                    msg = {
                        'success': 0,
                        'error_code': 20005,
                    }
                    logger.error(msg)
                    self.write(msg)
            else:
                logger.debug('method_action is none')
                handle = getattr(BaseHandle, self.request.method.lower())
                handle(self, *args, **kw)
    except tornado.httpclient.HTTPError as e:
        print(e)
        msg = {
            'success': 0,
            'error_code': 10102,
        }
        logger.error(msg)
        self.write(msg)

class RecordersHandle(GeneralHandle):
    """
    GET confs/{conf_id}/recorders
    POST confs/{conf_id}/recorders
    """
    handle = recorderHandle
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getrecorderlist.lua',
        'converter': api2mc.getrecorderlist,
    }

    _post = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTREC_REQ',
            'converter': api2mc.startrecorder,
        },
        'ack': {
            'msg': 'CMU_CM_STARTREC_ACK',
            'converter': mc2api.startrecack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTREC_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class RecorderInsHandle(GeneralHandle):
    """
    GET confs/{conf_id}/recorders/{rec_id}
    DELETE confs/{conf_id}/recorders/{rec_id}
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getrecorderstate.lua',
        'converter': api2mc.getrecorderstate,
    }

    _delete = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPREC_REQ',
            'converter': api2mc.recorderoperator,
        },
        'ack': {
            'msg': 'CMU_CM_STOPREC_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPREC_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class RecorderStateHandle(GeneralHandle):
    """"
    PUT confs/{conf_id}/recorders/{rec_id}/state
    """
    _put_pause = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_PAUSEREC_REQ',
            'converter': api2mc.recorderoperator,
        },
        'ack': {
            'msg': 'CMU_CM_PAUSEREC_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_PAUSEREC_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _put_resume = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_RESUMEREC_REQ',
            'converter': api2mc.recorderoperator,
        },
        'ack': {
            'msg': 'CMU_CM_RESUMEREC_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_RESUMEREC_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['value'] == 1:
            await self.handle(self._put_pause, *args)
        else:
            await self.handle(self._put_resume, *args)

class PlayHandle(GeneralHandle):
    """
    PUT /api/v1/vc/confs/{conf_id}/playback
    GET /api/v1/vc/confs/{conf_id}/playback
    """

    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getplaystate.lua',
        'converter': api2mc.getplaystate,
    }

    _put = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STARTPLAY_REQ',
            'converter': api2mc.startplay,
        },
        'ack': {
            'msg': 'CMU_CM_STARTPLAY_ACK',
            'converter': mc2api.startplayack,
        },
        'nack': {
            'msg': 'CMU_CM_STARTPLAY_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class PlayStateHandle(GeneralHandle):
    """
    PUT /api/v1/vc/confs/{conf_id}/playback/state

    """
    _put_pause = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_PAUSEPLAY_REQ',
            'converter': api2mc.playoperator,
        },
        'ack': {
            'msg': 'CMU_CM_PAUSEPLAY_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_PAUSEPLAY_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _put_resume = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_RESUMEPLAY_REQ',
            'converter': api2mc.playoperator,
        },
        'ack': {
            'msg': 'CMU_CM_RESUMEPLAY_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_RESUMEPLAY_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    _put_stop = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_STOPPLAY_REQ',
            'converter': api2mc.playoperator,
        },
        'ack': {
            'msg': 'CMU_CM_STOPPLAY_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_STOPPLAY_NACK',
            'converter': mc2api.defaultnack,
        }
    }

    async def put(self, *args, **kw):
        params = self.get_argument('params')
        reqobj = json.loads(params)
        if reqobj['value'] == 1:
            await self.handle(self._put_pause, *args)
        elif reqobj['value'] == 2:
            await self.handle(self._put_resume, *args)
        else:
            await self.handle(self._put_stop, *args)

class PlayProgHandle(GeneralHandle):
    """
    PUT /api/v1/vc/confs/{conf_id}/playback/current_progress
    """
    _put = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_SEEK_REQ',
            'converter': api2mc.playprog,
        },
        'ack': {
            'msg': 'CMU_CM_SEEK_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_SEEK_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class ViplistHandle(GeneralHandle):
    """
    PUT /api/v1/vc/confs/{conf_id}/viplist
    GET /api/v1/vc/confs/{conf_id}/viplist
    """
    _get = {
        'action': 'redis',
        'lua': 'v1/lua/getviplist.lua',
        'converter': api2mc.getviplist,
    }

    _put = {
        'action': 'mqrpc',
        'mq': CM_MCU_MQ,
        'req': {
            'msg': 'CM_CMU_SETVIPLIST_REQ',
            'converter': api2mc.setviplist,
        },
        'ack': {
            'msg': 'CMU_CM_SETVIPLIST_ACK',
            'converter': mc2api.defaultack,
        },
        'nack': {
            'msg': 'CMU_CM_SETVIPLIST_NACK',
            'converter': mc2api.defaultnack,
        }
    }

class ForceBrdHandle(GeneralHandle):
    """
    PUT /api/v1/vc/confs/{conf_id}/forcebroadcast
    """
    _put = {
        'action': 'mqpush',
        'mq': CM_MCU_MQ,
        'cmd': {
            'msg': 'CM_CMU_FORCEBRD_CMD',
            'converter': api2mc.forcebroadcast,
        },
    }
class APIVersionHandle(GeneralHandle):
    """
    GET version
    """
    async def get(self, *args, **kw):
        msg = {
            'version': APIVersion,
            'success': 1,
        }
        self.write(msg)

class APILogLevelHandle(GeneralHandle):
    """
    GET loglevel
    """
    async def get(self, *args, **kw):
        reqobj = int(self.path_args[0])
        loglvl = (logging.NOTSET, logging.DEBUG, logging.INFO, logging.WARNING, logging.ERROR)
        if logging.DEBUG <= loglvl[reqobj] <= logging.FATAL:
            logger = logging.getLogger('API')
            logger.setLevel(loglvl[reqobj])
            msg = {
                'success': 1,
            }
        else:
            msg = {
                'success': 0,
                'error_code': 112230
            }
        self.write(msg)

pre_url = '/api/v1/vc/'

url = [
    (pre_url + 'confs', ConfListHandle),
    (pre_url + 'confs/([0-9]+)', ConfHandle),
#    (pre_url + 'confs/([0-9]+)/state', ConfState),
    (pre_url + 'confs/([0-9]+)/cascades', CascadesConfHandle),
    (pre_url + 'confs/([0-9]+)/cascades/([0-9\.]+)/mts', CascadesMtHandle),
    (pre_url + 'confs/([0-9]+)/mts', MtListHandle),
    (pre_url + 'confs/([0-9]+)/silence', ConfSilenceHandle),
    (pre_url + 'confs/([0-9]+)/mute', ConfMuteHandle),
    (pre_url + 'confs/([0-9]+)/delay', DelayConfHandle),
    (pre_url + 'confs/([0-9]+)/monitors', ConfMonitorHandle),
    (pre_url + 'confs/([0-9]+)/monitors/([0-9.]+)/([0-9]+)', ConfSigleMonitorHandle),
    (pre_url + 'confs/([0-9]+)/monitors_heartbeat', MonitorAliveHandle),
    (pre_url + 'confs/([0-9]+)/neediframe/monitors', NeedIFrameHandle),
    (pre_url + 'confs/([0-9]+)/safty', ConfSaftyHandle),
    (pre_url + 'confs/([0-9]+)/speaker', SpeakerHandle),
    (pre_url + 'confs/([0-9]+)/chairman', ChairManHandle),
    (pre_url + 'confs/([0-9]+)/upload', UploadMtHandle),
    (pre_url + 'confs/([0-9]+)/online_mts', MtsStateHandle),
    (pre_url + 'confs/([0-9]+)/inspections', InspectMtHandle),
    (pre_url + 'confs/([0-9]+)/inspections/([0-9]+)/([0-9]+)', CancelInspectMtHandle),
    (pre_url + 'confs/([0-9]+)/(?:cascades/(?:[0-9]+)/)?mts/([0-9\.]+)', MtHandle),
    (pre_url + 'confs/([0-9]+)/mts/([0-9\.]+)/silence', MtSlienceHandle),
    (pre_url + 'confs/([0-9]+)/mts/([0-9\.]+)/mute', MtMuteHandle),
    (pre_url + 'confs/([0-9]+)/mts/([0-9\.]+)/camera', MtCameraRCHandle),
    (pre_url + 'confs/([0-9]+)/mts/([0-9\.]+)/volume', MtVolumHandle),
    (pre_url + 'confs/([0-9]+)/vmps', VmpStartHandle),
    (pre_url + 'confs/([0-9]+)/vmps/([0-9]+)', VmpHandle),
    (pre_url + 'confs/([0-9]+)/mixs', StartMixerHandle),
    (pre_url + 'confs/([0-9]+)/mixs/([0-9]+)', MixerHandle),
    (pre_url + 'confs/([0-9]+)/mixs/([0-9]+)/members', MixerMembersHandle),
    (pre_url + 'confs/([0-9]+)/sms', ShortMsgHandle),
    (pre_url + 'confs/([0-9]+)/vad', ConfVadHandle),
    (pre_url + 'confs/([0-9]+)/dualstream', SpecDualkSrcHandle),
    (pre_url + 'confs/([0-9]+)/poll', ConfPollHandle),
    (pre_url + 'confs/([0-9]+)/poll/state', ConfPollStateHandle),
    (pre_url + 'hdus', PlatformHduListHandle),
    (pre_url + 'confs/([0-9]+)/hdus', ConfHduListHandle),
    (pre_url + 'confs/([0-9]+)/hdus/([0-9_]+)', HduHandle),
    (pre_url + 'confs/([0-9]+)/mtvmps', MtListVmpsHandle),
    (pre_url + 'confs/([0-9]+)/mtvmps/([0-9\.]+)', MtVmpsHandle),
    (pre_url + 'confs/([0-9]+)/hduvmps', HduListVmpsHandle),
    (pre_url + 'confs/([0-9]+)/hduvmps/([0-9_]+)', HduVmpsHandle),
    (pre_url + 'confs/([0-9]+)/recorders', RecordersHandle),
    (pre_url + 'confs/([0-9]+)/recorders/([0-9]+)', RecorderInsHandle),
    (pre_url + 'confs/([0-9]+)/recorders/([0-9]+)/state', RecorderStateHandle),
    (pre_url + 'confs/([0-9]+)/playback', PlayHandle),
    (pre_url + 'confs/([0-9]+)/playback/state', PlayStateHandle),
    (pre_url + 'confs/([0-9]+)/playback/current_progress', PlayProgHandle),
    (pre_url + 'confs/([0-9]+)/viplist', ViplistHandle),
    (pre_url + 'confs/([0-9]+)/forcebroadcast', ForceBrdHandle),
    (pre_url + 'version', APIVersionHandle),
    (pre_url + 'loglevel/([0-9])', APILogLevelHandle),
]
