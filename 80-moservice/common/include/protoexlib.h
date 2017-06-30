#ifndef _PROTOEXLIB_H_
#define _PROTOEXLIB_H_

#include <string>
#include "json.h"
#include "google/protobuf/message.h"

//
//提供Protobuf扩展操作 by zhouhang 
//
class ProtoExRet
{
public:
	ProtoExRet(bool ret, const std::string &error_str = "") : ret_(ret), error_str_(error_str)
	{
	}
	~ProtoExRet(){}
	void AppendErr(std::string append)
	{
		error_str_ += append;
	}
	std::string GetErrStr() const
	{
		return error_str_;
	}
	bool operator==(const bool &compair)
	{
		return ret_ == compair;
	}

private:
	bool ret_;
	std::string error_str_;
};

class ProtoEx
{
public:
	//
	//将Protobuf类序列化为Json
	//[参数]    message     [in]     源消息
	//         js_root    [out]    目标Json对象
	//[返回]                       是否成功
	//
	static ProtoExRet SerializeToJson(const google::protobuf::Message *message, Json::Value& js_root);   

	//
	//将Json解析为C++类
	//[参数]    message     [out]      目标消息
	//         js_root    [in]	    源Json对象
	//[返回]                         是否成功
	//
	static ProtoExRet ParseFromJson(google::protobuf::Message *message, const Json::Value& js_root);

	static std::string GetLastErrorStr();
private:
	//
	//反射输出 把类的每一个字段逐一转换为Json
	//        [参数]message      [in]		生成类指针
	//              descriptor   [in]		当前字段描述符
	//              reflection   [in]		当前类的反射
	//              jsVal		 [out]   	用于输出的Json对象
	//
	static ProtoExRet ReflectionOutput(
		const google::protobuf::Message *message, 
		const google::protobuf::FieldDescriptor *descriptor, 
		const google::protobuf::Reflection *reflection,
		Json::Value &js_value);

	//
	//反射输入 根据类的每一个字段，逐一从Json中解析数据
	//        [参数]message     [out]      生成类指针
	//              descriptor  [out]      当前字段描述符
	//              reflection  [out]      当前类的反射
	//              jsVal       [in]       用于解析的Json对象
	//
	static ProtoExRet ReflectionInput(
		google::protobuf::Message *message, 
		const google::protobuf::FieldDescriptor *descriptor, 
		const google::protobuf::Reflection *reflection,
		const Json::Value& jsVal);

	// 工具接口
	static std::string StringFormat(const char *format, ...);
	static std::string GetJsonType(Json::ValueType type);

};
#endif
