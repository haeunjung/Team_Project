#pragma once

#include "Engine_Header.h"

WOOJUN_BEGIN

class DLL CBaseObj
{
protected:
	int					m_iRefCount;
	const type_info*	m_TypeID;
	string				m_strTag;
	string				m_strTypeName;
	// Logic °ü·Ã bool°ª
	bool				m_IsEnable;
	bool				m_IsAlive;
public:
	const type_info* GetTypeID() const {
		return m_TypeID;
	}
	const char* GetTag() const {
		return m_strTag.c_str();
	}
	const char* GetTypeName() const {
		return m_strTypeName.c_str();
	}
	const string& GetTagStr() const;
	const string& GetTypeNameStr() const;
	////////////////////////////////////////
	bool GetIsEnable() const {
		return m_IsEnable;
	}
	bool GetIsAlive() const {
		return m_IsAlive;
	}
	template <class T>
	bool TypeEquals() {
		if (m_TypeID == &typeid(T))
			return true;

		return false;
	}
public:
	template <typename T>
	void SetTypeID() {
		m_TypeID = &typeid(T);
	}
	void SetTag(const string& _strTag);
	void SetTypeName(const string& _strTypeName); 
	////////////////////////////////////////
	void SetIsEnable(bool _IsActive);
	void Death();
public:
	int AddRef();
	int RemoveRef();
	int Release();
	int Remove();
protected:
	CBaseObj();
	virtual ~CBaseObj() = 0;
};

WOOJUN_END