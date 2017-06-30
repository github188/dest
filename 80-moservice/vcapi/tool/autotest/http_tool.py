#-*- coding: utf-8 -*-
import urllib.request
import http.cookiejar

cookie = http.cookiejar.CookieJar()
opener = urllib.request.build_opener(urllib.request.HTTPCookieProcessor(cookie))

class ApiRequest:
    uri = ''
    param = ''


def http_get(req):
    response = opener.open(req)
    return response.read()


def http_post(req):
    response = opener.open(req)
    return response.read()

def http_put(req):
    req.get_method = lambda: 'PUT'
    response = opener.open(req)
    return response.read()


def http_delete(req):
    req.get_method = lambda: 'DELETE'
    response = opener.open(req)
    return response.read()

# test
# print http_get('/test')
