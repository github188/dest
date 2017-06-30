import tornado.web
import tornado.httpclient
import tornado.autoreload
import redis
import tornadopika
import v1.vchandle
import v1.syshandle
import v1.temphandle
import time
import logging

logger = logging.getLogger('API')

class CheckHandle(tornado.web.RequestHandler):
    def get(self):
        self.write('running')

class VcApi(tornado.web.Application):
    def __init__(self, configs):
        url_set = [
            ('/check', CheckHandle),
        ]

        url_set += v1.vchandle.url
        url_set += v1.syshandle.url
        url_set += v1.temphandle.url

        curtime = str(time.time())

        self.config = configs
        logger.info(self.config)
        self.db = redis.StrictRedis(host=configs['redis']['ip'],
                                    port=configs['redis']['port'],
                                    password=configs['redis']['password'])
        self.luasha = {}
        rmq_string = "amqp://%s:%s@%s:%s" % \
                     (configs['mq']['username'], configs['mq']['password'],
                      configs['mq']['ip'], configs['mq']['port'])
        logger.info(rmq_string)
        self.rmq = tornadopika.RpcClient(rmq_string)
        self.rmq._exchange = 'api.rpc.ex'
        self.rmq._exchange_type = 'direct'
        self.rmq._routing_key = 'api.rpc.k' + curtime
        self.rmq._queue = 'api.rpc.q' + curtime
        self.rmq.run()
        self.httpclient = tornado.httpclient.AsyncHTTPClient()
        tornado.web.Application.__init__(self, url_set, debug=False, gzip=True)
