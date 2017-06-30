
#include "inc.h"
#include "jsonwrap.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

namespace JsonWrap
{
	class CJsonValue::CJsonValueImpl
	{
	public:
		CJsonValueImpl(const int ival)
		{ 
			ostringstream oss;
			oss << ival;
			m_pjValue = json_string(oss.str().c_str());
			m_valueType = emJsonString;
		}
		CJsonValueImpl(const double dval)
		{
			ostringstream oss;
			oss << dval;
			m_pjValue = json_string(oss.str().c_str());
			m_valueType = emJsonString;
		}
		CJsonValueImpl(const char* pchVal)
		{
			m_pjValue = json_string(pchVal);
			m_valueType = emJsonString;
		}
		CJsonValueImpl(const bool bVal)
		{
			ostringstream oss;
			oss << bVal;
			m_pjValue = json_string(oss.str().c_str());
			m_valueType = emJsonString;
		}
		CJsonValueImpl(const enum EnumJsonValueType emValueType)
		{
			m_valueType = emValueType;
			if (emValueType == emJsonArray)
			{
				m_pjValue = json_array();
			}
			else if (emValueType == emJsonObject)
			{
				m_pjValue = json_object();
			}
			else if (emValueType == emJsonString)
			{
				m_pjValue = json_string("");
			}
		}

		BOOL32 PushBack(const json_t_ptr& cJsonPtr)
		{
			if (m_valueType != emJsonArray)
			{
				return false;
			}
			json_array_append_s(m_pjValue, cJsonPtr);
			return TRUE;
		}
		BOOL32 Insert(const std::string& strName, const json_t_ptr& cJsonPtr)
		{
			if (m_valueType != emJsonObject)
			{
				return false;
			}
			json_object_set_s(m_pjValue, strName.c_str(), cJsonPtr);
			return TRUE;
		}
		const char* ToString()
		{
			return json_dumps(m_pjValue, 0);
		}
		json_t_ptr m_pjValue;
	private:
		EnumJsonValueType m_valueType;
	};

	//CJsonValue
	CJsonValue::CJsonValue(const int ival)
	{
		m_valueImpl = new CJsonValueImpl(ival);
	}
	CJsonValue::CJsonValue(const double dval)
	{
		m_valueImpl = new CJsonValueImpl(dval);
	}
	CJsonValue::CJsonValue(const char* pchVal)
	{
		m_valueImpl = new CJsonValueImpl(pchVal);
	}
	CJsonValue::CJsonValue(const bool bVal)
	{
		m_valueImpl = new CJsonValueImpl(bVal);
	}
	CJsonValue::CJsonValue(const EnumJsonValueType emType)
	{
		m_valueImpl = new CJsonValueImpl(emType);
	}
	CJsonValue::~CJsonValue()
	{
		delete m_valueImpl;
	}
	BOOL32 CJsonValue::PushBack(const CJsonValue& cJsonValue)
	{
		return m_valueImpl->PushBack(cJsonValue.m_valueImpl->m_pjValue);
	}
	BOOL32 CJsonValue::Insert(const std::string& strName, const CJsonValue& cJsonValue)
	{
		return m_valueImpl->Insert(strName, cJsonValue.m_valueImpl->m_pjValue);
	}
	const s8* CJsonValue::ToString()
	{
		return m_valueImpl->ToString();
	}

	//CJsonString
	CJsonString::CJsonString(const int ival) :CJsonValue(ival) {}
	CJsonString::CJsonString(const double dval):CJsonValue(dval){}
	CJsonString::CJsonString(const char* pchval):CJsonValue(pchval){}
	CJsonString::CJsonString(const bool bVal) : CJsonValue(bVal){}

	//CJsonArray
	CJsonArray::CJsonArray() : CJsonValue(emJsonArray) {}
	BOOL32 CJsonArray::PushBack(const CJsonValue& cJsonValue)
	{
		return CJsonValue::PushBack(cJsonValue);
	}

	//CJsonObject
	CJsonObject::CJsonObject() : CJsonValue(emJsonObject) {}
	CJsonObject& CJsonObject::operator[](const std::string& strName)
	{
		m_strCurItem = strName;
		return *this;
	}
	CJsonObject& CJsonObject::operator=(const CJsonValue& cJsonValue)
	{
		Insert(m_strCurItem, cJsonValue);
		return *this;
	}
	CJsonObject& CJsonObject::operator=(const CJsonObject& cJsonValue)
	{
		Insert(m_strCurItem, cJsonValue);
		return *this;
	}
	BOOL32 CJsonObject::Insert(const std::string& strName, const CJsonValue& cJsonValue)
	{
		return CJsonValue::Insert(strName, cJsonValue);
	}

	//CJson
	//CJson::CJson(const char* strJsonBuf){}
	const char* CJson::ToString()
	{
		return CJsonObject::CJsonValue::ToString();
	}
}
