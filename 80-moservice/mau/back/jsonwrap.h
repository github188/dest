
#ifndef _JSONWRAP_
#define _JSONWRAP_

#include <string>

namespace JsonWrap
{
	enum EnumJsonValueType{emJsonString, emJsonObject, emJsonArray};

	class CJsonValue
	{
	public:
		CJsonValue(const int ival); 
		CJsonValue(const double dval);
		CJsonValue(const char* pchVal); 
		CJsonValue(const bool bVal);
		virtual ~CJsonValue() = 0;
	private:
		CJsonValue(const enum EnumJsonValueType emType);
		BOOL32 PushBack(const CJsonValue& cJsonValue);
		BOOL32 Insert(const std::string& strName, const CJsonValue& cJsonValue);
		const char* ToString();
	private:
		class CJsonValueImpl;
		CJsonValueImpl* m_valueImpl;
		friend class CJsonSting;
		friend class CJsonArray;
		friend class CJsonObject;
		friend class CJson;
	};

	class CJsonString : public CJsonValue
	{
	public:
		CJsonString(const int ival);
		CJsonString(const double dval);
		CJsonString(const char* pchval);
		CJsonString(const bool bVal);
	};

	class CJsonArray : public CJsonValue
	{
	public:
		CJsonArray();
		BOOL32 PushBack(const CJsonValue& cJsonValue);
	};

	class CJsonObject : public CJsonValue
	{
	public:
		CJsonObject();
		CJsonObject& operator[](const std::string& strName);
		CJsonObject& operator=(const CJsonValue& cJsonValue);
		CJsonObject& operator=(const CJsonObject& cJsonValue);
		BOOL32 Insert(const std::string& strName, const CJsonValue& cJsonValue);
	private:
		std::string m_strCurItem;
	};

	class CJson : public CJsonObject
	{
	public:
		const char* ToString();
	};
} 

#endif

//
//class CJsonArray
//{
//public:
//	CJsonArray()
//	{
//		m_pjArray = json_array();
//	}
//	CJsonArray(const json_t_ptr& pjArray) :m_pjArray(pjArray) {}
//	template<typename T>
//	BOOL32 Insert(const T& tVal)
//	{
//		std::ostringstream oss;
//		oss << tVal;
//		json_array_append_s( m_pjArray, json_string(oss.str().c_str()));
//		return TRUE;
//	}
//	BOOL32 Insert(const json_t_ptr& cJson)
//	{
//		json_array_append_s(m_pjArray, cJson);
//		return TRUE;
//	}
//	json_t_ptr Get(const u32 dwIndex)
//	{
//		return json_array_get_s( m_pjArray, dwIndex);
//	}
//	u32 Size()
//	{
//		return json_array_size(m_pjArray);
//	}
//	operator const json_t_ptr&()
//	{
//		return m_pjArray;
//	}
//	const json_t_ptr& GetArryPtr() const { return m_pjArray; }
//private:
//	json_t_ptr m_pjArray;
//};
//
//class CJson
//{
//public:
//	CJson()
//	{
//		m_pjRoot = json_object();
//	}
//	CJson(const std::string& strJson)
//	{
//		LoadJson(strJson.c_str(), strJson.length(), m_pjRoot, FALSE);
//	}
//	CJson(const json_t_ptr& pjRoot) :m_pjRoot(pjRoot) {}
//	~CJson()
//	{
//		if (m_pjRoot)
//		{
//			//json_free(m_pjRoot);
//		}
//	}
//	CJson GetObj(const s8* pchObjName)
//	{
//		return (CJson)json_object_get_s(m_pjRoot, pchObjName);
//	}
//
//	CJsonArray GetArray(const s8* pchArraryName)
//	{
//		return json_object_get_s(m_pjRoot, pchArraryName);
//	}
//
//	template<typename T>
//	CJson& operator=(const T& tVal)
//	{
//		SetVal(m_strValueName.c_str(), tVal);
//		return *this;
//	}
//	CJson& operator=(const CJsonArray& cJarray)
//	{
//		SetVal(m_strValueName.c_str(), cJarray);
//		return *this;
//	}
//	CJson& operator=(const CJson& cJson)
//	{
//		SetVal(m_strValueName.c_str(), cJson);
//		return *this;
//	}
//	CJson& operator[](const s8* strName) 
//	{
//		m_strValueName = strName;
//		return *this;
//	}
//	CJson& Get(const s8* strName)
//	{
//		m_strValueName = strName;
//		return *this;
//	}
//	const s8* Get()
//	{
//		return json_string_value(m_pjRoot);
//	}
//	template<typename T>
//	T Get()
//	{
//		const s8* pchVal = json_string_value(m_pjRoot);
//		istringstream iss(pchVal);
//		T tVal;
//		iss >> tVal;
//		return tVal;
//	}
//
//	template<typename T>
//	operator T() 
//	{
//		T tVal;
//		GetVal(m_strValueName.c_str(), tVal);
//		return tVal;
//	}
//	operator const s8*() 
//	{
//		GetVal(m_strValueName.c_str(), &m_pchStr);
//		return m_pchStr;
//	}
//	operator const json_t_ptr&()
//	{
//		return m_pjRoot;
//	}
//	const json_t_ptr& GetPtr() const { return m_pjRoot; }
//	const s8* ToString()
//	{
//		return json_dumps(m_pjRoot, 0);
//	}
//public:
//	template<typename T>
//	BOOL32 SetVal(const std::string& strName, const T& tVal)
//	{
//		std::ostringstream oss;
//		oss << tVal;
//		ReturnIfFailed(SetStringToJson(m_pjRoot, strName.c_str(), oss.str().c_str()));
//		return TRUE;
//	}
//	BOOL32 SetVal(const std::string& strName, const CJsonArray& cJarray)
//	{
//		json_object_set_s(m_pjRoot, strName.c_str(), cJarray.GetArryPtr());
//		return TRUE;
//	}
//	BOOL32 SetVal(const std::string& strName, const CJson& cJson)
//	{
//		json_object_set_s(m_pjRoot, strName.c_str(), (CJson&)cJson);
//		return TRUE;
//	}
//	template<typename T>
//	BOOL32 GetVal(const std::string& strName, T* tVal)
//	{
//		ReturnIfFailed(GetStringFromJson(m_pjRoot, strName.c_str(), (const s8**)tVal));
//		return TRUE;
//	}
//	template<typename T>
//	BOOL32 GetVal(const std::string& strName, T& tVal)
//	{
//		const s8* pchVal = NULL;
//		ReturnIfFailed(GetStringFromJson(m_pjRoot, strName.c_str(), &pchVal));
//		std::istringstream iss(pchVal);
//		iss >> tVal;
//		return TRUE;
//	}
//
//	std::string m_strValueName;
//private:
//	json_t_ptr m_pjRoot;
//	s8* m_pchStr;
//};
//typedef CJson CJsonObject;