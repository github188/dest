import tornado.web
import tornado.httpclient
import tornado.gen
import redis.exceptions
import json
import logging

logger = logging.getLogger('API')


class BaseHandle(tornado.web.RequestHandler):
    @property
    def db(self):
        return self.application.db

    @property
    def mq(self):
        return self.application.rmq

    @property
    def http(self):
        return self.application.httpclient

    async def get_token(self):
#        sso_token = self.get_argument('account_token', '')
        sso_key = self.get_cookie('SSO_COOKIE_KEY', '')
        url = 'http://%s/sso/ssocxf/rest/sso/validationToken?ssoToken=%s' % \
              (self.application.config['auth']['ip'], sso_key)
        req = tornado.httpclient.HTTPRequest(url, 'POST')
        req.headers.add('Content-Type', 'application/json')
        req.headers.add('accept', 'application/json')
        req.body = '/r/n'
        logger.info('request headers %s, body %s', req.url, req.body)
        response = await self.http.fetch(req)
        return response.body

    async def valid_token(self):
        rsp = await self.get_token()
        logger.info(rsp)
        if rsp is not None:
            token = json.loads(rsp.decode())
            logger.info(token)
            if 'userDomainMoid' not in token:
                logger.error('valid_token not binded to userdomain!')
                self.set_status(403)
                return None

            return {
                'userdomainadmin': token['defaultUserDomainAdmin'] or token['userDomainAdmin'],
                'moid': token['userDomainMoid'],
                'e164': token['e164'],
                'limited': int(token['limited']),
                'admin': str(int(token['meetingAdmin'])),
                'creatormoid': token['moid'],
                'enableVRS': int(token['enableVRS']),
                'enableLive': int(token['enableLive']),
            }
        else:
            logger.error('valid_token response none!')
            self.set_status(500)
        return None

class GeneralHandle(BaseHandle):
    """
    format like this:
    _get = {
        'action': 'mqrpc',
        'mq': {
            'ex': '',
            'key': '',
        },
        'req': {
            'reqmsg': '',
            'converter': None,
        },
        'rsp': {
            'ackmsg': '',
            'nackmsg': '',
            'ackconverter': None,
        }
    }
    """
    _get = None
    _post = None
    _delete = None
    _put = None

    def prepare(self):
        """
        此函数用于对无原生DELETE/PUT支持的客户端提供
        _method=DELETE/PUT形式的支持
        """
        method = self.get_argument('_method', '')
        if method != '':
            self.request.method = method

    async def get(self, *args, **kw):
        await self.handle(self._get, *args, **kw)

    async def post(self, *args, **kw):
        await self.handle(self._post, *args, **kw)

    async def delete(self, *args, **kw):
        await self.handle(self._delete, *args, **kw)

    async def put(self, *args, **kw):
        await self.handle(self._put, *args, **kw)

    async def handle(self, method_action, *args, **kw):
        try:
            account_info = await self.valid_token()
            if account_info is not None:
                if method_action is not None:
                    action = getattr(self, method_action['action'])
                    await action(method_action, account_info, *args, **kw)
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

    async def mqpush(self, method_action, account_info, *args, **kw):
        reqobj = {}
        params = self.get_body_argument('params', '')
        if len(params) > 0:
            reqobj = json.loads(params)

        msg = method_action['cmd']['converter'](*args, reqobj, account_info)
        msg['type'] = method_action['cmd']['msg']
        logger.info(msg)
        self.mq.publish(method_action['mq']['ex'], method_action['mq']['key'], json.dumps(msg))

        rtn = {
            'success': 1,
        }
        self.write(rtn)

    async def mqrpc(self, method_action, account_info, *args, **kw):
        reqobj = {}
        arguments = self.request.arguments
        if self.request.method == 'GET':
            for arg in arguments:
                reqobj[arg] = self.get_argument(arg, '')
        else:
            params = self.get_body_argument('params', '')
            if len(params) > 0:
                reqobj = json.loads(params)

        msg = method_action['req']['converter'](*args, reqobj, account_info)
        msg['type'] = method_action['req']['msg']
        logger.info(msg)
        mqrsp = await tornado.gen.Task(self.mq.call,
                                       method_action['mq']['ex'],
                                       method_action['mq']['key'],
                                       json.dumps(msg),
                                       time_out=10)
        if mqrsp is None:
            self.write_error(500)
            logger.error('response is none')
        else:
            if len(mqrsp) > 0:
                mqrspobj = json.loads(mqrsp.decode())
                if mqrspobj['type'] == method_action['ack']['msg']:
                    rsp = method_action['ack']['converter'](mqrspobj)
                else:
                    rsp = method_action['nack']['converter'](mqrspobj)
                    rsp['success'] = 0
                self.write(rsp)
                logger.info(rsp)
            else:
                self.write_error(500)

    async def evallua(self, filename, args):
        if filename not in self.application.luasha:
            file = open(filename, encoding='utf-8')
            luascript = file.read()
            sha = self.db.script_load(luascript)
            self.application.luasha[filename] = sha
        else:
            sha = self.application.luasha[filename]

#       if the sha of the scripts does not exist, reload it.
        try:
            msg = self.db.evalsha(sha, 0, *args)
        except redis.exceptions.NoScriptError:
            del self.application.luasha[filename]
            msg = await self.evallua(filename, args)
        except Exception as e:
            raise e

        return msg

    async def redis(self, method_action, account_info, *args, **kw):
        reqobj = {}
        arguments = self.request.arguments
        if self.request.method == 'GET':
            for arg in arguments:
                reqobj[arg] = self.get_argument(arg, '')
        params = method_action['converter'](account_info, *args, reqobj, **kw)
        msg = await self.evallua(method_action['lua'], params)
        logger.info(msg)
        self.write(msg.decode())
