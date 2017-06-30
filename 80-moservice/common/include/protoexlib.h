#ifndef _PROTOEXLIB_H_
#define _PROTOEXLIB_H_

#include <string>
#include "json.h"
#include "google/protobuf/message.h"

//
//�ṩProtobuf��չ���� by zhouhang 
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
	//��Protobuf�����л�ΪJson
	//[����]    message     [in]     Դ��Ϣ
	//         js_root    [out]    Ŀ��Json����
	//[����]                       �Ƿ�ɹ�
	//
	static ProtoExRet SerializeToJson(const google::protobuf::Message *message, Json::Value& js_root);   

	//
	//��Json����ΪC++��
	//[����]    message     [out]      Ŀ����Ϣ
	//         js_root    [in]	    ԴJson����
	//[����]                         �Ƿ�ɹ�
	//
	static ProtoExRet ParseFromJson(google::protobuf::Message *message, const Json::Value& js_root);

	static std::string GetLastErrorStr();
private:
	//
	//������� �����ÿһ���ֶ���һת��ΪJson
	//        [����]message      [in]		������ָ��
	//              descriptor   [in]		��ǰ�ֶ�������
	//              reflection   [in]		��ǰ��ķ���
	//              jsVal		 [out]   	���������Json����
	//
	static ProtoExRet ReflectionOutput(
		const google::protobuf::Message *message, 
		const google::protobuf::FieldDescriptor *descriptor, 
		const google::protobuf::Reflection *reflection,
		Json::Value &js_value);

	//
	//�������� �������ÿһ���ֶΣ���һ��Json�н�������
	//        [����]message     [out]      ������ָ��
	//              descriptor  [out]      ��ǰ�ֶ�������
	//              reflection  [out]      ��ǰ��ķ���
	//              jsVal       [in]       ���ڽ�����Json����
	//
	static ProtoExRet ReflectionInput(
		google::protobuf::Message *message, 
		const google::protobuf::FieldDescriptor *descriptor, 
		const google::protobuf::Reflection *reflection,
		const Json::Value& jsVal);

	// ���߽ӿ�
	static std::string StringFormat(const char *format, ...);
	static std::string GetJsonType(Json::ValueType type);

};
#endif
