from http_tool import http_get as GET
from http_tool import http_post as POST
from http_tool import http_delete as DELETE
from http_tool import http_put as PUT
from http_tool import ApiRequest as Request
from collections import OrderedDict

import urllib.parse
import urllib.request
import traceback
import auto_reader
import json
import os
import urllib
import time
import random

serverip = '172.16.185.233'
appname = 'TrueLink'
appkey = '12345678'

username = 'keda_user_admin1'
password = '888888'


configs = {
    'conf_id': '7779005',
    'conf_id_high': '1231231',
    'conf_id_low':'123123'
}



Inspection_item = [
    "start_time",
    "end_time",
#    "duration",
    "video_name",
    "port",
    "conf_id",
    "e164"
]


api_file = [
    'json_auto/kaoji.json'
    ]

#param_for_delete = {}

#param_for_put = {}


def splice_uri(uri):
    old_list = ['{conf_id}','{conf_id_high}','{conf_id_low}']
    replace_list = [configs['conf_id'],configs['conf_id_high'],configs['conf_id_low']]
    for index in range(len(old_list)):
        if uri.find(old_list[index]) != -1:
            uri = uri.replace(old_list[index], replace_list[index])
    return uri

def encode_get_uri(uri, param):
    temp_dict = json.loads(param)
    if len(temp_dict) > 0:
        uri += '?'
        uri += urllib.parse.urlencode(temp_dict)
    return uri

def change_param(param,method,title):
    rectime = time.strftime('%Y%m%d%H%M%S', time.localtime(time.time()))
    param_for_post = {
        "video_name": "filename" + rectime,  # 修改录像名
        "mts1": [
          {
            "account": configs["conf_id"],
            "account_type": 5,
            "bitrate": 1048,
            "protocol": 0
          }
        ],
        "mts2": [
          {
            "account": configs["conf_id_low"],
            "account_type": 5,
            "bitrate": 1048,
            "protocol": 0
          }
        ]
    }
    if param != '':
        param = json.loads(param)
        if method == 'POST':
            for key in param.keys():
                if key in param_for_post:
                    param[key]=param_for_post[key]
        for key in param.keys():
            if key == "mts1":
                param["mts"] = param_for_post[key]
                del param[key]
            if key == "mts2":
                param["mts"] = param_for_post[key]
                del param[key]
#        elif method == 'PUT':
#            for key in param.keys():
#                if key in param_for_put:
#                    param[key] = param_for_put[key]
#        elif method == 'DELETE':
#            for key in param.keys():
#                if key in param_for_delete:
#                    param[key] = param_for_delete[key]
        param = json.dumps(param)
    return param

def check_response(test_api,response):
    for key,value in test_api.items():
        if  key in response:
            if  key not in Inspection_item:
                if isinstance(value, list) and len(value) != 0:
                    if isinstance(test_api[key][0],dict):
                        for index in range(len(test_api[key])):
                            if key in response:
                                if len(response[key]) != 0:
                                    if False == check_response(test_api[key][index],response[key][index]):
                                        return False
                                else:
                                    print(key,"in json is not empty")
                                    return False
                            else:
                                print(key,"in response is not in json")
                                return False
                    else:
                        if response[key] not in value:
                            print(key,"=",response[key],"is not in json's range")
                            return False
                elif isinstance(value, dict) and value != {}:
                    if key in response:
                        if False == check_response(test_api[key], response[key]):
                            return False
                    else:
                        print(key, "in response is not in json")
                        return False
                else:
                    if key not in response:
                        print(key,"is not in response")
                        return False
                    if test_api[key] != response[key]:
                        print(key,"=",response[key],"is not equal to",test_api[key],"in json")
                        return False
        else:
            print(key,"is not in response")
            return False
    return True

def main():
    table = auto_reader.ApiTable()
    for index in range(len(api_file)):
        ret = table.load_apis(api_file[index])
        if ret is False:
            print('\nFailed to load %s!...' % (api_file[index]))
            os.system('pause')
            return 0
    api_count = table.get_api_count()
    data = {'oauth_consumer_key': appname,
            'oauth_consumer_secret': appkey}
    post_data = urllib.parse.urlencode(data)
    uri = 'http://' + serverip + '/api/v1/system/token'
    req = urllib.request.Request(uri, post_data.encode())
    response = POST(req)
    tokenrsp = json.loads(response.decode())
    account_token = tokenrsp['account_token']
    #   'account_token':account_token,
    uri = 'http://' + serverip + '/api/v1/system/login'
    data = {'account_token': account_token,
            'username': username,
            'password': password}
    post_data = urllib.parse.urlencode(data)
    #    uri ='http://' + loginip + '/sso/ssocxf/rest/sso/userLogin'
    req = urllib.request.Request(uri, post_data.encode())
    #    req.add_header('Content-Type', 'application/json')
    response = POST(req)
    print(response)

    #index = 0
    n = 0
    i = 0
    while 1:
        for index in range(api_count):
            test_api = table.get_api(index)
            method = test_api.method
            uri = test_api.uri
            uri = splice_uri(uri)
            title = test_api.title
            param = test_api.param
            param = json.dumps(param)
            auto_response = test_api.response
            auto_response = json.dumps(auto_response)

            if method == 'GET' and param != '':
                uri = encode_get_uri(uri, param)
                param = ''

            url = 'http://' + serverip + uri
            if method == 'GET':
                url += '?account_token=' + account_token
                request = urllib.request.Request(url)
            else:
                param = change_param(param, method,title)
                post_data = {
                    'account_token': account_token,
                    'params': param,
                }
            request = urllib.request.Request(url, urllib.parse.urlencode(post_data).encode())

            time.sleep(30)

            response = ''
            print('\nREQUEST %s %s ...\n' % (method, request.get_full_url()))
            if param != "":
                print('parma = ',param)
            if request.data is not None:
                print(request.data)
            if method == 'POST':
                try:
                    response = POST(request)
                except:
                    print('POST Exception!\n')
                    print(traceback.print_exc())
            elif method == 'DELETE':
                try:
                    response = DELETE(request)
                except:
                    print('DELETE Exception!\n')
                    print(traceback.print_exc())
            elif method == 'PUT':
                try:
                    response = PUT(request)
                except:
                    print('PUT Exception!\n')
                    print(traceback.print_exc())
            else:
                try:
                    response = GET(request)
                except:
                    print('GET Exception!\n')
                    print(traceback.print_exc())

            print('RESPONSE:\n%s\n' % (response))
            if response == '':
                print('method no exist!')
                break
            response = bytes.decode(response)
            response = json.loads(response)
            auto_response=json.loads(auto_response)
            if uri == "/api/v1/mc/confs":
                n = n + 1
                if n == 1:
                    conf_id_high = response["conf_id"]
                    configs["conf_id_high"] = conf_id_high
                elif n == 2:
                    conf_id = response["conf_id"]
                    configs["conf_id"] = conf_id
                elif n == 3:
                    conf_id_low = response["conf_id"]
                    configs["conf_id_low"] = conf_id_low
            if True == check_response(auto_response, response):
                print('test %d OK!' % (index + 1), test_api.title)
            else:
                print('test %d failed!' % (index + 1),test_api.title)

            index += 1
        i = i + 1
        print("循环次数：",i)

if __name__ == '__main__':
    main()