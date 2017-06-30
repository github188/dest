#encoding=utf-8
import re
import hashlib
import pymysql
import config
import logging

logger = logging.getLogger('API')

class api2mc:
    """
    Converters from APIs to internal parameters
    """
    openmode = {
        0: '1',
        1: '0',
    }
    vrstype = {
        'vrs2000': 309,
        'vrs4000': 310,
    }
    apimttype = {
        5: '1',
        6: '3',
        7: '1',
    }
    videoformat = {
        1: '97',
        2: '31',
        3: '34',
        4: '106',
        5: '106',
        6: '108'
    }
    videores = {
        1: '2',
        2: '3',
        3: '5',
        12: '32',
        13: '35',
        14: '15',
        15: '31',
        16: '62'
    }
    audioformat = {
        1: '96',
        2: '98',
        3: '99',
        4: '8',
        5: '0',
        6: '2',
        7: '9',
        8: '4',
        9: '15',
        10: '18',
        11: '13',
        12: '127',
        13: '102',
        14: '103'
    }

    @staticmethod
    def getrecinfo():
        try:
            connect = pymysql.connect(host=config.configs['apdb']['ip'],
                                      user=config.configs['apdb']['username'],
                                      passwd=config.configs['apdb']['password'],
                                      db='ap',
                                      cursorclass=pymysql.cursors.DictCursor,
                                      charset='utf8')
            cursor = connect.cursor()
            sql = 'SELECT device_ip FROM server_info WHERE ' \
                  'server_type IN (%d, %d) AND device_ip != "" AND enable = 1 ' \
                  'ORDER BY server_type ASC ' % (api2mc.vrstype['vrs2000'], api2mc.vrstype['vrs4000'])
            reclist = cursor.execute(sql)
            for recinfo in cursor.fetchmany(reclist):
                logging.info(recinfo)
                recorder = recinfo['device_ip'].split(';')
                pattern = re.compile('(.*):(.*)')
                match = pattern.match(recorder[0])
                if match:
                    recip = match.groups()[1]
                    logging.info('get recorder ip is %s' % recip)
                    break
            else:
                recip = '127.0.0.1'
            cursor.close()
            connect.close()
            return recip
        except pymysql.DatabaseError as e:
            logger.error('execute sql failed!')

    @staticmethod
    def strencryption(strdata):
        originalstr = hashlib.md5()
        originalstr.update(strdata.encode('utf-8'))
        originalstr.digest()
        return originalstr.hexdigest()

    @staticmethod
    def conflist(account_info, reqparam, *args, **kw):
        if 'start' not in reqparam or int(reqparam['start']) < 0:
            reqparam['start'] = 0
        if 'count' not in reqparam or int(reqparam['count']) < 0:
            reqparam['count'] = 10
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            account_info['creatormoid'],
            account_info['admin'],
            reqparam['start'],
            reqparam['count'],
        ]
        return msg

    @staticmethod
    def conf(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            account_info['creatormoid'],
            conf_id,
            account_info['admin'],
        ]
        return msg

    @staticmethod
    def confstate(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getcascadeconf(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getcascademts(account_info, conf_id, cascade_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
            cascade_id,
        ]
        return msg

    @staticmethod
    def getspeaker(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getinspectionlist(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getmonitorlist(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getmonitorinfo(account_info, conf_id, dst_ip, dst_port, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
            dst_ip,
            dst_port,
        ]
        return msg

    @staticmethod
    def getconfhdulist(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getchairman(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getupload(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getmixer(account_info, conf_id, mix_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
            mix_id,
        ]
        return msg

    @staticmethod
    def getconfmtlist(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getmtinfo(account_info, conf_id, mt_id, *args, **kw):
        mtindentify = api2mc.mtid2mtidentify(mt_id)
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
            mtindentify['mcuidx'],
            mtindentify['mtid'],
        ]
        return msg

    @staticmethod
    def getvmpinfo(account_info, conf_id, vmp_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
            vmp_id,
        ]
        return msg

    @staticmethod
    def getpollinfo(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getvadinfo(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getdualstream(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def gethduchninfo(account_info, conf_id, hdu_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
            hdu_id,
        ]
        return msg

    @staticmethod
    def mtid2mtidentify(mtid):
        pattern = re.compile('(.*)\.(.*)$')
        match = pattern.match(mtid)
        if match:
            mt = match.groups()
            mt_identify = {
                'mcuidx': mt[0],
                'mtid': mt[1],
            }
        elif mtid != '':
            mt_identify = {
                'mcuidx': '',
                'mtid': mtid,
            }
        else:
            mt_identify = {
                'mcuidx': '',
                'mtid': '',
            }
        return mt_identify

    @staticmethod
    def delmt(conf_id, mt_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mt': [{
                'mtidentify': api2mc.mtid2mtidentify(mt_id),
            }]
        }
        return msg

    @staticmethod
    def addmts(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mt': [],
        }
        if 'mts' in apimsg:
            for mt in apimsg['mts']:
                msg['mt'].append({
                    'mtNO': mt['account'],
                    'mttype': api2mc.apimttype[mt['account_type']],
                    'bitrate': str(mt['bitrate']),
                    'protocol': str(mt['protocol']),
                    'forcecall': str(mt['forced_call']) if 'forced_call' in mt else '0',
                })
        return msg

    @staticmethod
    def addcascadesmt(conf_id, cascades_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'smcu': {
                'mtidentify': {
                    'mcuidx': cascades_id,
                    'mtid': '0',
                },
            },
            'mt': [],
        }
        if 'mts' in apimsg:
            for mt in apimsg['mts']:
                msg['mt'].append({
                    'mtNO': mt['account'],
                    'mttype': api2mc.apimttype[mt['account_type']],
                    'bitrate': str(mt['bitrate']),
                    'protocol': str(mt['protocol']),
                    'forcecall': str(mt['forced_call']) if 'forced_call' in mt else '0',
                })
        return msg

    @staticmethod
    def delmts(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mt': [],
        }
        if 'mts' in apimsg:
            for mt in apimsg['mts']:
                msg['mt'].append({
                    'mtidentify': api2mc.mtid2mtidentify(mt['mt_id']),
                })
        return msg

    @staticmethod
    def callmt(conf_id, mt_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mt': [{
                'mtidentify': api2mc.mtid2mtidentify(mt_id),
            }],
        }
        return msg

    @staticmethod
    def setmts(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mt': [],
        }
        if 'mts' in apimsg:
            for mt in apimsg['mts']:
                msg['mt'].append({
                    'mtidentify': api2mc.mtid2mtidentify(mt['mt_id']),
                    'forcecall': str(mt['forced_call']) if 'forced_call' in mt else '0',
                })
        return msg

    @staticmethod
    def inspectmt(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mediamode': str(apimsg['mode']),
            'srcmt': {
                'mtidentify': api2mc.mtid2mtidentify(apimsg['src']['mt_id']),
                'mtchnid': '1',
            },
            'dstmt': {
                'mtidentify': api2mc.mtid2mtidentify(apimsg['dst']['mt_id']),
                'mtchnid': '1',
            }
        }
        return msg

    @staticmethod
    def newinspectvmp(conf_id, mt_id, *args):
        msg = {
            'confE164': conf_id,
            'dstmt': {
                'mtidentify': api2mc.mtid2mtidentify(mt_id),
                'mtchnid': '1',
            }
        }
        return msg

    @staticmethod
    def inspectvmp(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'dstmt': {
                'mtidentify': api2mc.mtid2mtidentify(apimsg['dst']['mt_id']),
                'mtchnid': '1',
            }
        }
        return msg

    @staticmethod
    def newstopinspectmt(conf_id, mt_id, mode, *args):
        msg = {
            'confE164': conf_id,
            'mediamode': mode,
            'dstmt': {
                'mtidentify': api2mc.mtid2mtidentify(mt_id),
                'mtchnid': '1',
            }
        }
        return msg

    @staticmethod
    def stopinspectmt(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mediamode': str(apimsg['mode']),
            'dstmt': {
                'mtidentify': api2mc.mtid2mtidentify(apimsg['dst']['mt_id']),
                'mtchnid': '1',
            }
        }
        return msg

    @staticmethod
    def setchairman(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mt': {
                "mtidentify": api2mc.mtid2mtidentify(apimsg['mt_id']),
            },
        }
        return msg

    @staticmethod
    def startupload(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mtidentify': api2mc.mtid2mtidentify(apimsg['mt_id']),
        }
        return msg

    @staticmethod
    def sete164(conf_id, *args):
        msg = {
            'confE164': conf_id,
        }
        return msg

    @staticmethod
    def setspeaker(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mt': {
                'mtidentify': api2mc.mtid2mtidentify(apimsg['mt_id']),
            },
        }
        return msg

    @staticmethod
    def delayconf(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'delay': str(apimsg['delay_time'])
        }
        return msg

    @staticmethod
    def setmtchnl(conf_id, mt_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mt': {
                'mtidentify': api2mc.mtid2mtidentify(mt_id),
                'mtchnid': '1',
            }
        }
        return msg

    @staticmethod
    def startmonitor(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'monitorsrc': {
                'selectsrctype': '1',
                'mtidentify': api2mc.mtid2mtidentify(apimsg['src']['mt_id']),
                'vidsrcchn': '1' if apimsg['mode'] == 0 else '0',
                'audsrcchn': '1' if apimsg['mode'] == 1 else '0',
            }
        }
        if apimsg['mode'] == 0:
            msg['dstvidaddr'] = {
                'transportaddr': {
                    'ip': apimsg['dst']['ip'],
                    'port': str(apimsg['dst']['port']),
                }
            }

            msg['dstaudaddr'] = {
                'transportaddr': {
                    'ip': '0',
                    'port': '0',
                }
            }
        elif apimsg['mode'] == 1:
            msg['dstaudaddr'] = {
                'transportaddr': {
                    'ip': apimsg['dst']['ip'],
                    'port': str(apimsg['dst']['port']),
                }
            }

            msg['dstvidaddr'] = {
                'transportaddr': {
                    'ip': '0',
                    'port': '0',
                }
            }
        return msg

    @staticmethod
    def startmonitorvmp(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'dstvidcap': {
                'mediatype': api2mc.videoformat[apimsg['video_format']['format']],
                'mediabitrate': str(apimsg['video_format']['bitrate']),
                'mediares': api2mc.videores[apimsg['video_format']['resolution']],
                'mediaframe': str(apimsg['video_format']['frame']),
                'mediah264': '1' if apimsg['video_format']['format'] != 5 else '0',
            },
            'dstvidaddr': {
                'transportaddr': {
                    'ip': apimsg['dst']['ip'],
                    'port': str(apimsg['dst']['port']),
                }
            }
        }
        return msg

    @staticmethod
    def startmonitormix(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'dstaudaddr': {
                'transportaddr': {
                    'ip': apimsg['dst']['ip'],
                    'port': str(apimsg['dst']['port']),
                }
            },
            'audiotype': api2mc.audioformat[apimsg['audio_format']['format']],
            'audiochnlnum': str(apimsg['audio_format']['chn_num']),
        }
        return msg

    @staticmethod
    def stopmonitor(conf_id, dst_ip, dst_port, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'dstaddr': [{
                'transportaddr': {
                    'ip': dst_ip,
                    'port': dst_port,
                },
            }],
        }
        return msg

    @staticmethod
    def neediframe(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'transportaddr': {
                    'ip': apimsg['dst']['ip'],
                    'port': str(apimsg['dst']['port']),
                },
        }
        return msg

    @staticmethod
    def setconfsafty(conf_id, apimsg, *args):
        if apimsg['password'] == "":
            password = ""
        else:
            password = api2mc.strencryption(apimsg['password'])

        msg = {
            'confE164': conf_id,
            'openmode': api2mc.openmode[apimsg['safe_conf']],
            'nodisturb': str(apimsg['closed_conf']),
            'psw': password,
        }
        return msg

    @staticmethod
    def startvmp(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mode': str(apimsg['mode']),
            'vmpstyle': str(apimsg['layout']),
            'vmpbrdst': str(apimsg['broadcast']),
            'vmpaddalias': str(apimsg['show_mt_name']),
            'vmprim': '1',
            'voicehint': str(apimsg['voice_hint']),
            'vmpmembers': []
        }
        if 'members' in apimsg:
            for member in apimsg['members']:
                msg['vmpmembers'].append({
                    'chnidx': str(member['chn_idx']),
                    'vmptype': str(member['member_type']),
                    'mtidentify': api2mc.mtid2mtidentify(member['mt_id']),
                    'mtchnid': '1',
                })
        return msg

    @staticmethod
    def changevmp(conf_id, vmp_id, apimsg, *args):
        msg = api2mc.startvmp(conf_id, apimsg, *args)
        msg['vmpid'] = vmp_id
        return msg

    @staticmethod
    def stopvmp(conf_id, vmp_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'vmpid': vmp_id,
        }
        return msg

    @staticmethod
    def startvad(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mixmode': '3',
            'vacinterval': str(apimsg['vacinterval']),
        }
        return msg

    @staticmethod
    def stopvad(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mixmode': '3',
        }
        return msg

    @staticmethod
    def startmixer(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mixmode': str(apimsg['mode']),
            'mixmember': []
        }
        if 'members' in apimsg:
            for member in apimsg['members']:
                msg['mixmember'].append({
                    'chnidx': '1',
                    'mtidentify': api2mc.mtid2mtidentify(member['mt_id']),
                })
        return msg

    @staticmethod
    def stopmix(conf_id, mix_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mixid': mix_id,
            'mixmode': "1",
        }
        return msg

    @staticmethod
    def handlepollstate(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'pollmode': str(apimsg['mode']),
        }
        return msg

    @staticmethod
    def setmixmembers(conf_id, mix_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mixid': mix_id,
            'mixmember': []
        }
        for member in apimsg['members']:
            msg['mixmember'].append({
                'chnidx': '1',
                'mtidentify': api2mc.mtid2mtidentify(member['mt_id']),
            })
        return msg

    @staticmethod
    def startpoll(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'pollmode': str(apimsg['mode']),
            'pollnum': str(apimsg['num']),
            'keeptime': str(apimsg['keep_time']),
            'pollmt': [],
        }
        if 'members' in apimsg:
            for mt in apimsg['members']:
                msg['pollmt'].append({
                    'mtidentify': api2mc.mtid2mtidentify(mt['mt_id']),
                    'mtchnid': '1',
                })
        return msg

    @staticmethod
    def forcebroadcast(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'forcebrd': str(apimsg['force_broadcast']),
        }
        return  msg

    @staticmethod
    def sendmsg(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'msgtype': str(apimsg['type']),
            'rolltimes': str(apimsg['roll_num']),
            'rollrate': str(apimsg['roll_speed']),
            'msg': apimsg['message'],
            'mt': []
        }
        if 'mts' in apimsg:
            for mt in apimsg['mts']:
                msg['mt'].append({
                    'mtidentify': api2mc.mtid2mtidentify(mt['mt_id']),
                })
        return msg

    @staticmethod
    def startmtvmp(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mode': str(apimsg['mode']),
            'vmpstyle': str(apimsg['layout']),
            'mtchnid': '1',
            'mtidentify': api2mc.mtid2mtidentify(apimsg['mt_id']),
            'vmpaddalias': str(apimsg['show_mt_name']),
            'vmprim': '1',
            'voicehint': str(apimsg['voice_hint']),
            'vmpmembers': []
        }
        if 'members' in apimsg:
            for member in apimsg['members']:
                msg['vmpmembers'].append({
                    'chnidx': str(member['chn_idx']),
                    'vmptype': str(member['member_type']),
                    'mtidentify': api2mc.mtid2mtidentify(member['mt_id']),
                    'mtchnid': '1',
                })
        return msg

    @staticmethod
    def getmtlistvmps(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getmtvmps(account_info, conf_id, mt_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
            mt_id,
        ]
        return msg

    @staticmethod
    def gethduvmpslist(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def gethduvmps(account_info, conf_id, hdu_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
            hdu_id,
        ]
        return msg

    @staticmethod
    def starthduvmps(conf_id, apimsg, *args):
        hduinfo = api2mc.hduid2hduinfo(apimsg['hdu_id'])
        msg = {
            'confE164': conf_id,
            'vmpstyle': str(apimsg['layout']),
            'chnnlidx': hduinfo['chnnlidx'],
            'hduid': hduinfo['hduid'],
            'vmpaddalias': str(apimsg['show_mt_name']),
            'hduschedidx': '0',
            'mode': '1',
            'vmprim': '1',
            'hdumember': []
        }
        if 'members' in apimsg:
            for member in apimsg['members']:
                msg['hdumember'].append({
                    'subchnidx': str(member['chn_idx']),
                    'vmptype': str(member['member_type']),
                    'mtidentify': api2mc.mtid2mtidentify(member['mt_id']),
                    'mtchnid': '1',
                })
        return msg

    @staticmethod
    def changehduvmps(conf_id, hdu_id, apimsg, *args):
        hduinfo = api2mc.hduid2hduinfo(hdu_id)
        msg = {
            'confE164': conf_id,
            'vmpstyle': str(apimsg['layout']),
            'chnnlidx': hduinfo['chnnlidx'],
            'hduid': hduinfo['hduid'],
            'vmpaddalias': str(apimsg['show_mt_name']),
            'hduschedidx': '0',
            'mode': '1',
            'vmprim': '1',
            'hdumember': []
        }
        if 'members' in apimsg:
            for member in apimsg['members']:
                msg['hdumember'].append({
                    'subchnidx': str(member['chn_idx']),
                    'vmptype': str(member['member_type']),
                    'mtidentify': api2mc.mtid2mtidentify(member['mt_id']),
                    'mtchnid': '1',
                })
        return msg

    @staticmethod
    def stophduvmps(conf_id, hdu_id, apimsg, *args):
        hduinfo = api2mc.hduid2hduinfo(hdu_id)
        msg = {
            'confE164': conf_id,
            'chnnlidx': hduinfo['chnnlidx'],
            'hduid': hduinfo['hduid'],
        }
        return msg

    @staticmethod
    def changemtvmps(conf_id, mt_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mode': str(apimsg['mode']),
            'vmpstyle': str(apimsg['layout']),
            'mtchnid': '1',
            'mtidentify': api2mc.mtid2mtidentify(mt_id),
            'vmpaddalias': str(apimsg['show_mt_name']),
            'vmprim': '1',
            'voicehint': str(apimsg['voice_hint']),
            'vmpmembers': []
        }
        if 'members' in apimsg:
            for member in apimsg['members']:
                msg['vmpmembers'].append({
                    'chnidx': str(member['chn_idx']),
                    'vmptype': str(member['member_type']),
                    'mtidentify': api2mc.mtid2mtidentify(member['mt_id']),
                    'mtchnid': '1',
                })
        return msg

    @staticmethod
    def stopmtvmps(conf_id, mt_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mtidentify': api2mc.mtid2mtidentify(mt_id),
            'mtchnid': '1',
        }
        return msg

    @staticmethod
    def monitorkeepalive(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'dstaddrlist': []
        }
        for dst in apimsg['monitors']:
            msg['dstaddrlist'].append({
                'transportaddr': {
                    'ip': dst['ip'],
                    'port': str(dst['port']),
                }
            })
        return msg

    @staticmethod
    def specdualsrc(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mtidentify': api2mc.mtid2mtidentify(apimsg['mt_id']),
        }
        return msg

    @staticmethod
    def stopspecdualsrc(conf_id, mcu_idx, mt_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mtidentify': {
                "mcuidx": mcu_idx,
                "mtid": mt_id
            }
        }
        return msg

    @staticmethod
    def camerarc(conf_id, mt_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'mov': str(apimsg['type']),
            'mt': {
                'mtidentify': api2mc.mtid2mtidentify(mt_id),
                'mtchnid': '1',
            }
        }
        return msg

    @staticmethod
    def mtvolum(conf_id, mt_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'volume': str(apimsg['vol_value']),
            'codectype': str(apimsg['vol_mode']),
            'mtidentify': api2mc.mtid2mtidentify(mt_id),
            'chnid': '1',
        }
        return msg

    @staticmethod
    def hdulist(account_info, apimsg, *args):
        msg = {
            'moid': apimsg['moid'],
        }
        return msg

    @staticmethod
    def hduid2hduinfo(hdu_id):
        pattern = re.compile('(.*)_(.*)')
        hdu = pattern.search(hdu_id).groups()
        hduinfo = {
            'hduid': hdu[0],
            'chnnlidx': hdu[1],
        }
        return hduinfo

    @staticmethod
    def starthdu(conf_id, hdu_id, apimsg, *args):
        hduinfo = api2mc.hduid2hduinfo(hdu_id)
        msg = {
            'confE164': conf_id,
            'hduid': hduinfo['hduid'],
            'chnnlidx': hduinfo['chnnlidx'],
            'hduschedidx': '0',
            'hdustyle': str(apimsg['mode'] - 1),
            'hdumember': []
        }
        if apimsg['mode'] == 1:
            msg['hdumember'].append({
                'subchnidx': '0',
                'vmptype': apimsg['specific']['member_type'],
                'mtchnid': '0',
                'mttype': '1',
                'mtidentify': api2mc.mtid2mtidentify(apimsg['specific']['mt_id']),
            })
        elif apimsg['mode'] == 2:
            for mt in apimsg['spilt']['members']:
                msg['hdumember'].append({
                    'subchnidx': str(mt['chn_idx']),
                    'vmptype': '1',
                    'mtchnid': '0',
                    'mttype': '1',
                    'mtidentify': api2mc.mtid2mtidentify(mt['mt_id']),
                })
        return msg

    @staticmethod
    def newstarthdu(conf_id, apimsg, *args):
        hduinfo = api2mc.hduid2hduinfo(apimsg['hdu_id'])
        msg = {
            'confE164': conf_id,
            'hduid': hduinfo['hduid'],
            'chnnlidx': hduinfo['chnnlidx'],
            'hduschedidx': '0',
            'hdustyle': str(apimsg['mode'] - 1),
            'hdumember': []
        }
        if apimsg['mode'] == 1:
            msg['hdumember'].append({
                'subchnidx': '0',
                'vmptype': apimsg['specific']['member_type'],
                'mtchnid': '0',
                'mttype': '1',
                'mtidentify': api2mc.mtid2mtidentify(apimsg['specific']['mt_id']),
            })
        elif apimsg['mode'] == 2:
            for mt in apimsg['spilt']['members']:
                msg['hdumember'].append({
                    'subchnidx': str(mt['chn_idx']),
                    'vmptype': '1',
                    'mtchnid': '0',
                    'mttype': '1',
                    'mtidentify': api2mc.mtid2mtidentify(mt['mt_id']),
                })
        return msg

    @staticmethod
    def stophdu(conf_id, hdu_id, apimsg, *args):
        hduinfo = api2mc.hduid2hduinfo(hdu_id)
        msg = {
            'confE164': conf_id,
            'hduid': hduinfo['hduid'],
            'chnnlidx': hduinfo['chnnlidx'],
            'hduschedidx': '0',
            'hdustyle': str(apimsg['mode'] - 1),
        }
        return msg

    @staticmethod
    def deletehdu(conf_id, hdu_id, hdu_mode, *args):
        hduinfo = api2mc.hduid2hduinfo(hdu_id)
        msg = {
            'confE164': conf_id,
            'hduid': hduinfo['hduid'],
            'chnnlidx': hduinfo['chnnlidx'],
            'hduschedidx': '0',
            'hdustyle': hdu_mode,
        }
        return msg

    @staticmethod
    def newstophdu(conf_id, apimsg, *args):
        hduinfo = api2mc.hduid2hduinfo(apimsg['hdu_id'])
        msg = {
            'confE164': conf_id,
            'hduid': hduinfo['hduid'],
            'chnnlidx': hduinfo['chnnlidx'],
            'hduschedidx': '0',
            'hdustyle': str(apimsg['mode'] - 1),
        }
        return msg

    @staticmethod
    def starthdupoll(conf_id, hdu_id, apimsg, *args):
        hduinfo = api2mc.hduid2hduinfo(hdu_id)
        msg = {
            'confE164': conf_id,
            'hduid': hduinfo['hduid'],
            'chnnlidx': hduinfo['chnnlidx'],
            'hduschedidx': '0',
            'pollnum': apimsg['poll']['num'],
            'keeptime': apimsg['poll']['keep_time'],
            'pollmode': apimsg['poll']['mode'],
            'hdumember': []
        }
        for mt in apimsg['poll']['members']:
            msg['hdumember'].append({
                'mtchnid': '0',
                'mttype': '1',
                'mtidentify': api2mc.mtid2mtidentify(mt['mt_id']),
            })
        return msg

    @staticmethod
    def newstarthdupoll(conf_id, apimsg, *args):
        hduinfo = api2mc.hduid2hduinfo(apimsg['hdu_id'])
        msg = {
            'confE164': conf_id,
            'hduid': hduinfo['hduid'],
            'chnnlidx': hduinfo['chnnlidx'],
            'hduschedidx': '0',
            'pollnum': apimsg['poll']['num'],
            'keeptime': apimsg['poll']['keep_time'],
            'pollmode': apimsg['poll']['mode'],
            'hdumember': []
        }
        for mt in apimsg['poll']['members']:
            msg['hdumember'].append({
                'mtchnid': '0',
                'mttype': '1',
                'mtidentify': api2mc.mtid2mtidentify(mt['mt_id']),
            })
        return msg

    @staticmethod
    def stophdupoll(conf_id, hdu_id, *args):
        hduinfo = api2mc.hduid2hduinfo(hdu_id)
        msg = {
            'confE164': conf_id,
            'hduid': hduinfo['hduid'],
            'chnnlidx': hduinfo['chnnlidx'],
            'hduschedidx': '0',
        }
        return msg

    @staticmethod
    def getrecorderlist(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getrecorderstate(account_info, conf_id, rec_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
            rec_id,
        ]
        return msg

    @staticmethod
    def startrecorder(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'recname': apimsg['video_name'],
            'publishmode': str(apimsg['publish_mode']),
            'isanonymous': str(apimsg['anonymous']),
            'recmode': str(apimsg['recorder_mode']),
            'recid': '0',
            'isrecmainvideo': str(apimsg['main_stream']),
            'isrecdstream': str(apimsg['dual_stream']),
            'ismerge': '0',
            'vrsalias': {
                'aliastype': '1',
                'addr': api2mc.getrecinfo(),
                'vrsNO': ''
            },
            'mt': []
        }
        if apimsg['recorder_type'] == 2:
            for mtinfo in apimsg['members']:
                msg['mt'].append({
                    'mtidentify': api2mc.mtid2mtidentify(mtinfo['mt_id']),
                    'mtchnid': '1'
                })
        return msg

    @staticmethod
    def recorderoperator(conf_id, rec_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'recid': rec_id,
            'recmode': str(apimsg['recorder_mode'])
        }
        return msg

    @staticmethod
    def startplay(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'issecvideoplay': str(apimsg['play_dual_stream']),
            'filename': apimsg['file_name'],
            'fileid': str(apimsg['file_id']),
            'vrsalias': {
                'aliastype': '1',
                'addr': api2mc.getrecinfo(),
                'vrsNO': ''
            }
        }
        return msg

    @staticmethod
    def playoperator(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id
        }
        return msg

    @staticmethod
    def playprog(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'prog': {
                'curprog': str(apimsg['current_progress'])
            }
        }
        return msg

    @staticmethod
    def getplaystate(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def getviplist(account_info, conf_id, *args, **kw):
        msg = [
            account_info['moid'],
            account_info['e164'],
            str(account_info['limited']),
            conf_id,
        ]
        return msg

    @staticmethod
    def setviplist(conf_id, apimsg, *args):
        msg = {
            'confE164': conf_id,
            'vipmt': [],
        }

        if 'mts' in apimsg:
            for mt in apimsg['mts']:
                msg['vipmt'].append({
                    'mtidentify': api2mc.mtid2mtidentify(mt['mt_id']),
                })

        return  msg

class mc2api:
    """
    Converters from internal parameters to APIs
    """
    @staticmethod
    def defaultack(mcmsg):
        apimsg = {
            'success': 1
        }
        return apimsg

    @staticmethod
    def defaultnack(mcmsg):
        apimsg = {
            'error_code': int(mcmsg['errcode']) if 'errcode' in mcmsg else 0,
        }
        return apimsg

    @staticmethod
    def cascadesaddmtack(mcmsg):
        apimsg = {
            'success': 1,
            'mts': [],
        }

        if 'mt' in mcmsg:
            for mt in mcmsg['mt']:
                apimsg['mts'].append({
                    'account': mt['mtNO'],
                    'account_type': int(mt['mttype']) if mt['mttype'] != "" else 0,
                    'protocol': int(mt['protocol']) if mt['protocol'] != "" else 0,
                    'mt_id': int(mt['mtidentify']['mtid']) if mt['mtidentify']['mtid'] != "" else 0,
                    'cascades_id': int(mt['mtidentify']['mcuidx']) if mt['mtidentify']['mcuidx'] != "" else 0,
                })
        return apimsg

    @staticmethod
    def addmtack(mcmsg):
        apimsg = {
            'success': 1,
            'mts': [],
        }

        if 'mt' in mcmsg:
            for mt in mcmsg['mt']:
                apimsg['mts'].append({
                    'account': mt['mtNO'],
                    'account_type': int(mt['mttype']),
                    'protocol': int(mt['protocol']) if mt['protocol'] != "" else 0,
                    'mt_id': int(mt['mtidentify']['mtid']),
                })
        return apimsg

    @staticmethod
    def startrecack(mcmsg):
        apimsg = {
            'success': 1,
            'rec_id': int(mcmsg['recid']),
        }
        return apimsg

    @staticmethod
    def startplayack(mcmsg):
        apimsg = {
            'success': 1,
            'rec_id': int(mcmsg['vrsid']),
        }
        return apimsg

    @staticmethod
    def hdulist(mcmsg):
        apimsg = {
            'success': 1,
            'hdus': [],
        }

        for hdu in mcmsg['hdulist']:
            for chn in hdu['chninfo']:
                if int(chn['chnstatus']) == 0 or int(chn['chnstatus']) == 1:
                    apimsg['hdus'].append({
                        'hdu_id': hdu['hduid'] + '_' + chn['chnnlidx'],
                        'hdu_name': hdu['hdualias'],
                        'online': int(hdu['isonline']),
                        'occupy': int(chn['chnstatus']),
                    })
        return apimsg
