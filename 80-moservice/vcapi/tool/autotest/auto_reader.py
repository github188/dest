#!/usr/bin/python
#-*- coding: utf-8 -*-

import traceback
import json
from collections import OrderedDict

class Auto:
    uri = ''
    title = ''
    method = ''
    param = ''
    response = ''


def parse_api(json_api):
    new_api = Auto()
    new_api.uri = json_api['uri']
    print('Parsing ' + new_api.uri)
    new_api.title = json_api['title']
    api_method = json_api['method']
    if api_method is not None:
        new_api.method = json_api['method']
    new_api.param = json_api['param']
    new_api.response = json_api['response']
    return new_api

class ApiTable:
    title = ''
    api_list = list()

    def clear(self):
        del self.api_list[:]
        title = ''

    def load_apis(self, api_file_name):
        print('Reading ' + api_file_name)
        api_file = open(api_file_name, 'r', encoding='utf-8')
        file_text = ''
        try:
            file_text = api_file.read()#json中的内容
        except:
            print(traceback.print_exc())
            return False
        api_file.close()
        try:
            all_apis = json.loads(file_text, object_pairs_hook=OrderedDict)#将JSON字典转化格式，安顺序输出为item的形式
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
                    self.api_list.append(new_api)#由一个个json对象组成的列表
        return True

    def api_title(self):
        return self.title

    def get_api_count(self):
        return len(self.api_list)

    def get_api(self, index):
        return self.api_list[index]

def print_api(table):
    get_count = table.get_api_count()
    for index in range(get_count):
        api = table.get_api(index)
        print('%02d. %-6s %-40s %-s' % (index + 1, api.method, api.uri, api.title))

def test():
    table = ApiTable()
    table.load_apis('apis.json')
    print_api(table)