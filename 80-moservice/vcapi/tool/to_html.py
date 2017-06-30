#!/usr/bin/python
#-*- coding: utf-8 -*-

import json
import api_reader
from pyh import *
from configparser import ConfigParser
from collections import OrderedDict

class JsonToHtmlParam:
    title = '科达视讯平台会议业务API'
    h1 = '科达视讯平台会议业务API1'
    h2 = 'API列表：'
    version = 'V5.0.0.4.0.20170110'
    css = ['api.css']
    js = ['api.js']

def getCfgItems(filename):
        confFile = ConfigParser()
        confFile.read(filename, 'utf-8')
        sections = confFile.sections()
        for index in sections:
            if 'title' in confFile.options(index):
                title = h1 = confFile.get(index, 'title')
            if 'html_file' in confFile.options(index):
                html_file = confFile.get(index, 'html_file')
            if 'json_files' in confFile.options(index):
                files = confFile.get(index, 'json_files')
                files = files.replace(' ', '')
                json_files = files.split(',')
            if 'version' in confFile.options(index):
                version = confFile.get(index, 'version')
                print(version)
        return (title, h1, json_files, html_file, version)

def get_uri_table(title, uri, method, comments):
    uri_table = table(cl='uri_t', id='uri_table')
    title_row = uri_table << tr(id='title_row')
    title_row << th('名称')
    title_row << td(title)
    uri_row = uri_table << tr(id='uri_row')
    uri_row << th('URI')
    uri_row << td(uri)
    method_row = uri_table << tr(id='method_row')
    method_row << th('方法')
    method_row << td(method)
    comment_row = uri_table << tr(id='comment_row')
    comment_row << th('说明')
    comment_row << td(comments)
    return uri_table
			
def Recursive(param_table, param, recnum):
    style = '&nbsp'*4*recnum
    for (key, value) in param.items():
        param_row = param_table << tr(id='param_row')
        if isinstance(value, OrderedDict):
            param_row << td(style + key)
            if  '_name' in value:
                if isinstance(value['_name'],list):
                    for index in range(len(value['_name'])):
                        param_row << td(value['_name'][index])
                else:
                    param_row << td('')
                    param_row << td(value['_name'])
            Recursive(param_table, value, recnum + 1)
        elif key != '_name':
            param_row << td(style + key)
            if isinstance(value,list):
                for index in range(len(value)):
                    param_row << td(value[index])
            else:
                param_row << td("")
                param_row << td(value)

def get_param_table(param):
    param_table = table(cl='t', id='param_table', border=1)
    param_row = param_table << tr(id='param_title_row')
    if len(param) == 0:
        param_row << td('null')
        return param_table
    param_row << th('参数名称')
    param_row << th('数据类型',id="data_style")
    param_row << th('说明')
    Recursive(param_table, param, 0)
    return param_table

def get_example_table(example_text):
    example_dict = json.loads(example_text, object_pairs_hook=OrderedDict)
    example_div = div(id='example_div')
    example_table = table(cl='t', id='example_table', border=1)
    example_row = example_table << tr(id='example_row')
    example_text = json.dumps(example_dict, indent=2, sort_keys=False, ensure_ascii=False)
    example_text = example_text.replace('\n', '<br/>')
    example_text = example_text.replace(' ', '&nbsp')
    example_code = example_div << a(example_text, id='example_table')
    return example_div

def get_div_GET(api):
    div_gets = div(id='div_gets')
    div_name = api.method
    div_name += '_'
    div_name += api.uri
    div_get = div(id='div_get')
    div_gets << a(name=div_name)
    div_get << get_uri_table(api.title, api.uri, api.method, api.comments)
    div_get << p('参数说明', id='param_title')
    div_get << get_param_table(api.param_comments)
    if api.param_example != '':
        div_get << p('params示例', id='example_title')
        div_get << get_example_table(api.param_example)
    if len(api.param_example_comments) != 0:
        div_get << p('params说明', id='param_title')
        div_get << get_param_table(api.param_example_comments)
    div_get << p('成功回复示例', id='example_title')
    div_get << get_example_table(api.success_rsp_example)
    div_get << p('成功回复说明', id='param_title')
    div_get << get_param_table(api.success_rsp_comments)
    div_get << p('失败回复示例', id='example_title')
    div_get << get_example_table(api.fail_rsp_example)
    div_get << p('失败回复说明', id='param_title')
    div_get << get_param_table(api.fail_rsp_comments)
    div_gets << div_get
    return div_gets

def get_div_POST(api):
    div_posts = div(id='div_posts')
    div_name = api.method
    div_name += '_'
    div_name += api.uri
    div_post = div(id='div_post')
    div_posts << a(name=div_name)
    div_post << get_uri_table(api.title, api.uri, api.method, api.comments)
    div_post << p('参数说明', id='param_title')
    div_post << get_param_table(api.param_comments)
    if api.param_example != '':
        div_post << p('params示例', id='example_title')
        div_post << get_example_table(api.param_example)
    if len(api.param_example_comments) != 0:
        div_post << p('params说明', id='param_title')
        div_post << get_param_table(api.param_example_comments)
    div_post << p('成功回复示例', id='example_title')
    div_post << get_example_table(api.success_rsp_example)
    div_post << p('成功回复说明', id='param_title')
    div_post << get_param_table(api.success_rsp_comments)
    div_post << p('失败回复示例', id='example_title')
    div_post << get_example_table(api.fail_rsp_example)
    div_post << p('失败回复说明', id='param_title')
    div_post << get_param_table(api.fail_rsp_comments)
    div_posts << div_post
    return div_posts

def get_div_PUSH(api):
    div_pushs = div(id='div_pushs')
    div_name = api.method
    div_name += '_'
    div_name += api.uri
    div_push = div(id='div_push')
    div_pushs << a(name=div_name)
    div_push << get_uri_table(api.title, api.uri, '推送', api.comments)
    div_push << p('消息示例', id='example_title')
    div_push << get_example_table(api.success_rsp_example)
    div_push << p('消息说明', id='param_title')
    div_push << get_param_table(api.success_rsp_comments)
    div_pushs << div_push
    return div_pushs

def uricontent_list_div(api_table):
    uri_list_div = div(id='uri_list_div')
    api_count = api_table.get_api_count()
    count = 0
    for index in range(api_count):
        api = api_table.get_api(index)
        if api.enable is False:
            continue
        api_href = '#' + api.method + '_' + api.uri
        api_disp = '%d. %s'% (count + 1, api.title)
        uri_list_div << a(api_disp, href=api_href)
        uri_list_div << br()
        count += 1
    return uri_list_div

#def apidoc_introduction():

#def apiexample():

def json_to_html(json_files, html_file, param):
    page = PyH(param.title)
    page += meta(content='text/html', charset='utf-8')
    #css
    page.head += '<style type = "text/css">'
    for x in range(len(param.css)):
        css_file = open(param.css[x], 'r', encoding='utf-8')
        css_text = css_file.read()
        css_text = css_text.replace('\n', '')
        css_text = css_text.replace('\t', '')
        page.head += css_text
        css_file.close()
    page.head += '</style>'
    page.head += '<link rel="icon" href="api.ico" type="image/x-icon">'

    #h1
    div_title = page << div(id='div_title')
    div_title_inner = div_title << div(id='div_title_inner')
    div_title_inner << h1(param.h1)
    div_title_inner << h2(param.version)
    div_title_occupy = page << div(id='div_title_occupy')

    #content
    div_main = page << div(id='div_main')
    div_content = div_main << div(id='div_content')
    div_content_body = div_content << div(id='div_content_body')
    uri_table = api_reader.ApiTable()
    div_body = div_main << div(id='div_body')
    for index in range(len(json_files)):
        uri_table.clear()
        ret = uri_table.load_apis(json_files[index])
        if ret == False:
            print('Failed to load data from %s!' % (json_files[index]))
            return 0
        api_count = uri_table.get_api_count()
        if uri_table.api_title().strip() and api_count > 0:
            div_content_body << h3(uri_table.api_title())
            div_content_body << uricontent_list_div(uri_table)

        for apiindex in range(api_count):
            api = uri_table.get_api(apiindex)
            if api.enable:
                if api.method == 'GET':
                    div_body << get_div_GET(api)
                elif api.method == 'POST' or api.method == 'PUT' or api.method == 'DELETE':
                    div_body << get_div_POST(api)
                elif api.method == 'PUSH':
                    div_body << get_div_PUSH(api)
                div_body << br()
    #js
    page.body += '<script>'
    for x in range(len(param.js)):
        js_file = open(param.js[x], 'r')
        js_text = js_file.read()
        page.body += js_text
        js_file.close()
    page.body += '</script>'
    page.printOut(html_file)

def main():
    json_files = []
    html_file = ''
    param = JsonToHtmlParam()
    print(param.version)
    param.title, param.h1, json_files, html_file ,param.version= getCfgItems('tohtml.ini')
    json_to_html(json_files, html_file, param)

if __name__ == '__main__':
    main()
