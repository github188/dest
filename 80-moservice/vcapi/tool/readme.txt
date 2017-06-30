要求Python版本：3.5.1

/to_html.py   生成html文档，直接用python调用
/test.py      单元测试程序，直接用python运行

/pyh.py        提供写html功能
/http_tool.py  提供http请求接口
/api_reader.py 提供读取api配置文件功能
/api.js        html公用javascript
/api.css       html公用css


tohtml.ini配置说明：
[config]
title = api文档标题
json_files = json文件列表，以英文逗号分隔
html_file = 目标文件名


注意，由于当前部分数据结构有嵌套，不同嵌套里可能存在同名字段，在参数说明时，需体现出层次关系,否则同名字段会被覆盖。
如：
{
   "video_format":
   [
       {
           "format":1,     //视频格式
           "resloution":2, //分辨率
           "rate":128      //码率
       },
   ],
   "audio_format":
   {
     "format":1,     //音频格式
     "rate":128      //码率
   }
}
参数说明可以按如下方式编写:
"params_comments":
{
    "video_format": {
        "_name": ["object","视频参数"]			//_name为video_format的参数说明
        "format": ["int","视频格式"]，
        "resloution": ["int","视频分辨率"]，
        "rate":  ["int","视频码率"]
	},
    "audio_format": {
		"_name": ["object","音频参数"],		//_name为audio_format的参数说明
		"format": ["int","音频格式"],
		"rate":  ["int","音频码率"]
	}，
}

json配置文件格式
{
  "title":"模块名"
  "requests":                                //请求数组
  [                                      
    {
      "uri": "/confs",                       //uri
      "title":"获取会议列表",                //API标题
      "comments": "本接口用于会议列表",      //API注释
      "method":"GET",                        //请求方法GET POST DELETE PUT
   
      "param": {                             //api参数
        "comments":{                         //参数说明
          "account_token": ["str","软件权限token"],
          "params": "请求内容，以JSON形式发送，需进行UrlEncode"
        }
        "params_example": {                  //body内的params参数示例
          "moid": "2222222222222222"
        },
        "params_comments": {                 //body内的params参数说明
          "moid": "摩云id"
        }
      },
      "response":                            //api回复
      {                          
        "success":{                          //成功回复示例
          "example": {                       
            "conf_list": [
              {
                "name": "conf1",
                "conf_id": "123#456"
              },
              {
                "name": "conf2",
                "conf_id": "31#456"
              }
            ]
          },
          "comments": {                       //成功回复说明
            "conf_list": ["array","会议列表"],
            "name": ["str","会议名称"],
            "conf_id": ["str","会议号"]
          }
        },
        "fail": {  
          "example": {                        //失败回复示例
             "success": 0,
             "error_code": 0
          },
          "comments": {                       //失败回复说明
            "success": ["int","是否成功"],
             "error_code": ["int","错误码"]
          }
        }
      }
    },
    ....
  ]
}