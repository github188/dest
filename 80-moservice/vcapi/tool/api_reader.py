#!/usr/bin/python
#-*- coding: utf-8 -*-

import traceback
import json
import re
from collections import OrderedDict

class Api:
    uri = ''
    comments = ''
    title = ''
    method = ''
    param_comments = OrderedDict()
    param_example = ''
    param_example_comments = OrderedDict()
    success_rsp_example = ''
    success_rsp_comments = OrderedDict()
    fail_rsp_example = ''
    fail_rsp_comments = OrderedDict()
    enable = False

def parse_api(json_api):
    new_api = Api()
    new_api.uri = json_api['uri']
    #print('Parsing ' + new_api.uri)
    new_api.title = json_api['title']
    new_api.comments = json_api['comments']
    new_api.enable = json_api['enable']
    api_method = json_api['method']
    if api_method is not None:
        new_api.method = json_api['method']
    api_param = json_api['param']
    if api_param is not None:
        new_api.param_comments = api_param['comments']
        if 'params_example' in api_param:
            new_api.param_example = json.dumps(api_param['params_example'])
        if 'params_comments' in api_param:
            new_api.param_example_comments = api_param['params_comments']
    api_rsp = json_api['response']
    if api_rsp is not None:
        success_rsp_example = api_rsp['success']['example']
        new_api.success_rsp_example = json.dumps(success_rsp_example)
        new_api.success_rsp_comments = api_rsp['success']['comments']
        fail_rsp_example = api_rsp['fail']['example']
        new_api.fail_rsp_example = json.dumps(fail_rsp_example)
        new_api.fail_rsp_comments = api_rsp['fail']['comments']
    return new_api

class ApiTable:
    title = ''
    api_list = list()

    def clear(self):
        del self.api_list[:]
        title = ''

    def load_apis(self, api_file_name):
        print('Reading ' + api_file_name)
        try:
            api_file = open(api_file_name, 'r', encoding='utf-8')
        except:
            input("json文件加载失败，请检查json路径及配置文件是否正确,回车以继续：")
            print(traceback.print_exc())
        file_text = ''
        try:
            file_text = api_file.read()
        except:
            print(traceback.print_exc())
            return False
        api_file.close()
        try:
            all_apis = json.loads(file_text, object_pairs_hook=OrderedDict)
        except:
            print(traceback.print_exc())
            return False
        if 'title' in all_apis.keys():
            self.title = all_apis['title']
        for key in all_apis.keys():
            if key != 'title':
                apis = all_apis[key]
                for index in range(len(apis)):
                    new_api = parse_api(apis[index])
                    if new_api.enable:
                        self.api_list.append(new_api)
        return True

    def api_title(self):
        return self.title

    def get_api_count(self):
        return len(self.api_list)

    def get_api(self, index):
        return self.api_list[index]

def print_api(table):
    get_count = table.get_api_count()
    digit = len(str(get_count))
    if get_count % 2 !=1:
        get_count_1 = int(get_count/2)
        for index in range(get_count_1):
            if index != 0:
                api = table.get_api(2*index)
                api_1 = table.get_api(2*index+1)
                if len(api.title) < 18:
                    zhPattern = re.findall(u'[\u4e00-\u9fa5]|[\u3002\uff1b\uff0c\uff1a\u201c\u201d\uff08\uff09\u3001\uff1f\u300a\u300b]+',api.title)
                    long = 18 - len(api.title) + len(api.title) - len(('').join(zhPattern))
                    space = str(" "*long)
            else:
                api = table.get_api(0)
                api_1 = table.get_api(1)
                if len(api.title) < 18:
                    zhPattern = re.findall(u'[\u4e00-\u9fa5]|[\u3002\uff1b\uff0c\uff1a\u201c\u201d\uff08\uff09\u3001\uff1f\u300a\u300b]+', api.title)
                    long = 18 - len(api.title) + len(api.title) - len(('').join(zhPattern))
                    space = str(" "*long)
            print('%0{d}d. %-20s'.format(d=digit) %(2 * (index + 1) - 1, api.title) + space + '%0{d}d. %-20s'.format(d=digit) %(2 * (index + 1), api_1.title))
    else:
        get_count_1 = int(get_count / 2)
        for index in range(get_count_1):
            if index != 0:
                api = table.get_api(2 * index)
                api_1 = table.get_api(2 * index + 1)
                if len(api.title) < 18:
                    zhPattern = re.findall(u'[\u4e00-\u9fa5]|[\u3002\uff1b\uff0c\uff1a\u201c\u201d\uff08\uff09\u3001\uff1f\u300a\u300b]+',api.title)
                    long = 18 - len(api.title) + len(api.title) - len(('').join(zhPattern))
                    space = str(" "*long)
            else:
                api = table.get_api(0)
                api_1 = table.get_api(1)
                if len(api.title) < 18:
                    zhPattern = re.findall(u'[\u4e00-\u9fa5]|[\u3002\uff1b\uff0c\uff1a\u201c\u201d\uff08\uff09\u3001\uff1f\u300a\u300b]+', api.title)
                    long = 18 - len(api.title) + len(api.title) - len(('').join(zhPattern))
                    space = str(" "*long)
            print('%0{d}d. %-20s'.format(d=digit) % (2 * (index + 1) - 1, api.title) + space + '%0{d}d. %-20s'.format(d=digit) % (2 * (index + 1), api_1.title))
        print('%0{d}d. %-20s'.format(d=digit) %(get_count,table.get_api(get_count-1).title))



def test():
    table = ApiTable()
    table.load_apis('apis.json')
    print_api(table)