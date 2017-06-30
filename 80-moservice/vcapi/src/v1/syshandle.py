from .generalhandle import GeneralHandle
import tornado.httpclient
import tornado.httputil
import xml.etree.ElementTree as ET
import logging

logger = logging.getLogger('API')

class TokenHandle(GeneralHandle):
    """
    POST system/token
    """
    async def post(self, *args, **kw):
        url = 'http://' + self.application.config['auth']['ip'] + '/api5/accountToken'
        req = tornado.httpclient.HTTPRequest(url, 'POST', self.request.headers, self.request.body)
        del req.headers['Host']
        try:
            rsp = await self.http.fetch(req)

            if rsp is not None:
                root = ET.fromstring(rsp.body.decode())
                token = root.find('accountToken')
                if token is not None:
                    msg = {
                        'success': 1,
                        'account_token': token.text,
                    }
                    logger.info(msg)
                    self.write(msg)
                elif 'errorCode' in root.tag:
                    code = root.find('code')
                    msg = {
                        'success': 0,
                        'error_code': int(code.text),
                    }
                    logger.error('Get token failed! %s' % msg['error_code'])
                    self.write(msg)
        except tornado.httpclient.HTTPError as e:
            self.set_status(500)
            logger.error(e)


class LoginHandle(GeneralHandle):
    """
    POST system/login
    """
    async def post(self, *args, **kw):
        url = 'http://' + self.application.config['auth']['ip'] + '/api5/login'
        req = tornado.httpclient.HTTPRequest(url, 'POST', self.request.headers, self.request.body)
        del req.headers['Host']
        try:
            rsp = await self.http.fetch(req)

            if rsp is not None:
                root = ET.fromstring(rsp.body.decode())
                success_node = root.find('success')
                if (success_node is not None) and (success_node.text == 'true'):
                    username_node = root.find('username')
                    msg = {
                        'success': 1,
                        'username': username_node.text,
                    }
                    logger.info(msg)
                    cookies = rsp.headers.get_list('Set-Cookie')
                    for cookie in cookies:
                        self.set_header('Set-Cookie', cookie)
                    self.write(msg)
                elif 'errorCode' in root.tag:
                    code = root.find('code')
                    msg = {
                        'success': 0,
                        'error_code': int(code.text),
                    }
                    logger.error('username or password login failed! %s' % msg['error_code'])
                    self.write(msg)
        except tornado.httpclient.HTTPError as e:
            self.set_status(500)
            logger.error(e)


class Heartbeat(GeneralHandle):
    """
    POST system/heartbeat
    """
    async def post(self, *args, **kw):
        url = 'http://' + self.application.config['auth']['ip'] + '/api5/users/keepHeartbeat'
        req = tornado.httpclient.HTTPRequest(url, 'POST', self.request.headers, self.request.body)
        del req.headers['Host']
        try:
            rsp = await self.http.fetch(req)

            if rsp is not None:
                root = ET.fromstring(rsp.body.decode())
                success_node = root.find('success')
                if (success_node is not None) and (success_node.text == 'true'):
                    msg = {
                        'success': 1
                    }
                elif 'errorCode' in root.tag:
                    code = root.find('code')
                    msg = {
                        'success': 0,
                        'error_code': int(code.text),
                    }
                    logger.error('post hearbeat failed! %s' % msg['error_code'])
                self.write(msg)
        except tornado.httpclient.HTTPError as e:
            print(e)
            logger.error(e)

pre_url = '/api/v1/system/'
url = [
    (pre_url + 'token', TokenHandle),
    (pre_url + 'login', LoginHandle),
    (pre_url + 'heartbeat', Heartbeat),
]
