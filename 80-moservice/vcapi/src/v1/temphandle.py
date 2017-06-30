from .generalhandle import GeneralHandle
import tornado.httpclient
import logging

logger = logging.getLogger('API')

class AmcapiHandle(GeneralHandle):
    """
    POST system/token
    """

    async def httpproxy(self, suburl):
        try:
            account_info = await self.valid_token()
            if account_info is not None:
                if account_info['userdomainadmin']:
                    requrl = 'http://'\
                             + self.application.config['amc-api']['ip']\
                             + '/amcapi/' + suburl\
                             + '?user_domain_moid=' + account_info['moid']
                    if self.request.query != '':
                        requrl += '&' + self.request.query
                    if self.request.method == 'GET':
                        reqbody = None
                    else:
                        reqbody = self.request.body
                    req = tornado.httpclient.HTTPRequest(
                        requrl, self.request.method,
                        self.request.headers, reqbody, allow_nonstandard_methods=True)
                    rsp = await self.http.fetch(req)
                    self.set_status(rsp.code)
                    self.proxy_set_header(rsp.headers)
                    logger.info(rsp.body)
                    self.write(rsp.body)
                else:
                    logger.error('get valid token not have userdomianadmin')
                    self.set_status(403)
        except tornado.httpclient.HTTPError:
            logger.error('amcapi httpproxy get valid token failed!')
            self.set_status(500)

    def proxy_set_header(self, headers):
        for (k, v) in headers.get_all():
            if k == 'Server':
                pass
            elif k == 'Transfer-Encoding':
                if v == 'chunked':
                    # 如果设置了分块传输编码，但是实际上代理这边已经完整接收数据
                    # 到了浏览器端会导致(failed)net::ERR_INVALID_CHUNKED_ENCODING
                    pass
            elif k == 'Location':
                self.set_header('Location', self._rewrite_location(v))
            elif k.lower() == 'content-length':
                # 代理传输过程如果采用了压缩，会导致remote传递过来的content-length与实际大小不符
                # 会导致后面self.write(response.body)出现错误
                # 可以不设置remote headers的content-length
                # "Tried to write more data than Content-Length")
                # HTTPOutputError: Tried to write more data than Content-Length
                pass
            elif k.lower() == 'content-encoding':
                # 采用什么编码传给请求的客户端是由Server所在的HTTP服务器处理的
                pass
            elif k == 'Set-Cookie':
                # Set-Cookie是可以有多个，需要一个个添加，不能覆盖掉旧的
                self.add_header(k, v)
            elif k == 'Content-Disposition':
                # todo 下载文件，中文文件名，会有编码问题
                # 中文文件名采用GB2312编码时会乱码
                self.set_header(k, v)
            else:
                self.set_header(k, v)

    get = post = delete = put = httpproxy


pre_url = '/api/v1/temp/'
url = [
    (pre_url + '(.*)', AmcapiHandle),
]
