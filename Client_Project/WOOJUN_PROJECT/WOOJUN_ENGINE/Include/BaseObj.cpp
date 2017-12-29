#include "BaseObj.h"

WOOJUN_USING

const string & CBaseObj::GetTagStr() const
{
	return m_strTag;
}

const string & CBaseObj::GetTypeNameStr() const
{
	return m_strTypeName;
}

void CBaseObj::SetTag(const string & _strTag)
{
	m_strTag = _strTag;
}

void CBaseObj::SetTypeName(const string & _strTypeName)
{
	m_strTypeName = _strTypeName;
}

void CBaseObj::SetIsEnable(bool _IsEnable)
{
	m_IsEnable = _IsEnable;
}

void CBaseObj::Death()
{
	m_IsAlive = false;
}

int CBaseObj::AddRef()
{	
	++m_iRefCount;
	return m_iRefCount;
}

int CBaseObj::Release()
{
	--m_iRefCount;

	if (0 == m_iRefCount)
	{
		delete this;
		return 0;
	}
	return m_iRefCount;
}

int CBaseObj::Remove()
{
	m_iRefCount = 0;

	delete this;
	return 0;
}

CBaseObj::CBaseObj() : 
	m_iRefCount(1),
	m_IsEnable(true),
	m_IsAlive(true)
{
}


CBaseObj::~CBaseObj()
{
}
