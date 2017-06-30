# !/usr/bin/python
# -*- coding: utf-8 -*-

from http_tool import http_get as GET
from http_tool import http_post as POST
from http_tool import http_delete as DELETE
from http_tool import http_put as PUT
from http_tool import ApiRequest as Request
from collections import OrderedDict
from configparser import ConfigParser

import urllib.parse
import urllib.request
import traceback
import api_reader

import json
import tempfile
import os
import urllib
# import re


def make(uri, param=0):
    req = Request()
    req.uri = uri
    req.param = param
    return req


def confs(moid):
    uri = '/confs?moid=' + moid
    req = Request()
    req.uri = uri
    return req


def conf(confid):
    req = Request()
    req.uri = '/confs/' + confid
    return req


def ask_usr_edit(uri, param, comments):
    text_editor = 'notepad'
#   text_editor = '"C:\\Program Files (x86)\\Notepad++\\notepad++.exe" '
    param_dict = {}
    if param != '':
        param_dict = json.loads(param, object_pairs_hook=OrderedDict)
    temp_dict = {'uri': uri, 'param': param_dict, 'comments': comments}
    temp_text = json.dumps(temp_dict, indent=2, ensure_ascii=False)
#    temp_text = temp_text.decode('unicode-escape').encode('gbk')
#   consts
#    const_text = '\n\n\n============================consts============================\n\n\n'
#    const_file =  open('api_v1_vc_const.proto', 'r')
#    if const_file:
#        const_text += const_file.read()
#        const_file.close()
#   temp_text += const_text
#   print temp_text
    temp_file_path = tempfile.mktemp()
    temp_file = open(temp_file_path, 'w', encoding='utf-8-sig')
    temp_file.write(temp_text)
    temp_file.close()
    os.system(text_editor + ' ' + temp_file_path)
    temp_file = open(temp_file_path, 'r', encoding='utf-8-sig')
    temp_text = temp_file.read()
#   idx = temp_text.index('=========================')
#   temp_dict = json.loads(temp_text[0:idx], encoding='gbk')
#   temp_file.close(),
    temp_dict = json.loads(temp_text)
    uri = temp_dict['uri']
    param_dict = temp_dict['param']
    if len(param_dict) > 0:
        param = json.dumps(param_dict)
#   os.remove(temp_file_path)
    return uri, param


def encode_get_uri(uri, param):
    temp_dict = json.loads(param)
    if len(temp_dict) > 0:
        uri += '?'
        uri += urllib.parse.urlencode(temp_dict)
    return uri


def getCfgItems(filename):
    confFile = ConfigParser()
    confFile.read(filename, 'utf-8')
    sections = confFile.sections()
    for index in sections:
        if 'serverip' in confFile.options(index):
            serverip = confFile.get(index, 'serverip')
        if 'appname' in confFile.options(index):
            appname = confFile.get(index, 'appname')
        if 'appkey' in confFile.options(index):
            appkey = confFile.get(index, 'appkey')
        if 'username' in confFile.options(index):
            username = confFile.get(index, 'username')
        if 'password' in confFile.options(index):
            password = confFile.get(index, 'password')
        if 'api_file' in confFile.options(index):
            files = confFile.get(index, 'api_file')
            files = files.replace(' ', '')
            api_file = files.split(',')
        info_list = [serverip,appname,appkey,username,password,api_file]
    return info_list


def main():
    table = api_reader.ApiTable()

    try:
        info_list = getCfgItems('test_info.ini')
    except:
        input("读取配置文件异常，回车以继续：")
        input(traceback.print_exc())

    api_file = info_list[5]

    for index in range(len(api_file)):
        ret = table.load_apis(api_file[index])
        if ret is False:
            print('\nFailed to load %s!...' % (api_file[index]))
            os.system('pause')
            continue
        else:
            print("load succeed")
    api_count = table.get_api_count()

    while 1:
        serverip = info_list[0]
        appname = info_list[1]
        appkey = info_list[2]
        username = info_list[3]
        password = info_list[4]

        data = {'oauth_consumer_key': appname,
                'oauth_consumer_secret': appkey}
        post_data = urllib.parse.urlencode(data)
        uri = 'http://' + serverip + '/api/v1/system/token'
        req = urllib.request.Request(uri, post_data.encode())
        try :
            response = POST(req)
        except :
            input("ip地址配置有误无法连接或restapi为正常启动，请检查配置文件，回车以继续："+"\n")
            print(traceback.print_exc())
            continue
        tokenrsp = json.loads(response.decode())
        if tokenrsp['success'] == 0:
            input("appname或appkey 配置有误，获取token失败，请检查配置文件：")
            continue
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
        loginrsp = json.loads(response.decode())
        print(loginrsp)
        if loginrsp['success'] == 0:
            input("用户名或密码配置有误，登录失败，请重检查配置文件：")
            continue
        else:
            print("登陆成功！")
            input("输入回车继续：")

        while 1:
            print('\n\n0. reload api from json file')
            api_reader.print_api(table)
            try:
                test_case = int(input('Select case to test:'))
            except:
                print('invalid input\n')
                continue
        # reload json
            if test_case == 0:
                table.clear()
                table.load_apis(api_file)
                api_count = table.get_api_count()
                continue

            if test_case > api_count or test_case <= 0:
                print('invalid case %d\n' % test_case)
                continue

            test_api = table.get_api(test_case - 1)
            method = test_api.method
            uri = test_api.uri
            param = test_api.param_example
            param_comments = test_api.param_example_comments
            uri, param = ask_usr_edit(uri, param, param_comments)

            if method == 'GET' and param != '':
                uri = encode_get_uri(uri, param)

            url = 'http://' + serverip + uri

            if method == 'GET':
                if param != '':
                    url += '&account_token=' + account_token
                else:
                    url += '?account_token=' + account_token
                request = urllib.request.Request(url)
            else:
                post_data = {
                    'account_token': account_token,
                    'params': param,
                }
#            if method == 'DELETE':
 #               post_data['_method'] = 'DELETE'
                request = urllib.request.Request(url, urllib.parse.urlencode(post_data).encode())

            response = ''
            print('\nREQUEST %s %s ...\n' % (method, request.get_full_url()))
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

            print('RESPONSE:\n%s\n' % response)
            input('输入回车继续：')

if __name__ == '__main__':
    main()
