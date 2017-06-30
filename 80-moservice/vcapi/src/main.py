#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import v1.app
import config
import tornado.httpserver
import time
import tornado.ioloop
import logging
import logging.handlers
import os
import sys
import platform
from tornado.log import access_log, gen_log, app_log

logger = logging.getLogger('API')
def handle_exception(exc_type, exc_value, exc_traceback):
    if issubclass(exc_type, KeyboardInterrupt):
        sys.__excepthook__(exc_type, exc_value, exc_traceback)
        return
    logger.error("Uncaught exception", exc_info=(exc_type, exc_value, exc_traceback))

def runapp():
    os.chdir(os.path.dirname(__file__))
    config.load('../config/conf.ini')

    #set log
    if platform.system() == 'Windows':
        LOG_PATH = 'log'
    else:
        LOG_PATH = '/usr/log'

    if False == os.path.exists(LOG_PATH):
        os.mkdir(LOG_PATH)
    access_log.propagate = False
    app_log.propagate = False
    gen_log.propagate = False
    handler = logging.handlers.RotatingFileHandler(LOG_PATH + '/api.log', maxBytes=1024 * 1024 * 10, backupCount=5)
    datefmt = '%Y-%m-%d %H:%M:%S'
    fmt = '%(levelname)-8s%(asctime)s %(name) -4s%(funcName) -12s %(lineno) -5d: %(message)s'
    formatter = logging.Formatter(datefmt=datefmt, fmt=fmt)
    handler.setFormatter(formatter)
    logger = logging.getLogger('API')
    logger.addHandler(handler)
    gen_log.addHandler(handler)
    app_log.addHandler(handler)
    access_log.addHandler(handler)
    logger.setLevel(logging.DEBUG)
    sys.excepthook = handle_exception

    MeetingApi = v1.app.VcApi(config.configs)
    logger.info(MeetingApi.db)
    MeetingApiServer = tornado.httpserver.HTTPServer(MeetingApi)

    need_connect = True

    while(need_connect):
        try:
            MeetingApiServer.listen(config.configs['local']['port'])
            need_connect = False
        except OSError as e:
            print('Bind Port %s Failed:' % (config.configs['local']['port']))
            print(e)
            time.sleep(1)

    tornado.ioloop.IOLoop.current().start()

if __name__ == '__main__':
    import setproctitle
    setproctitle.setproctitle('restapi')
    runapp()

