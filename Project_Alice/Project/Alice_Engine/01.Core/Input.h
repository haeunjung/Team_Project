#pragma once
#include "../Engine_Header.h"

WOOJUN_BEGIN

typedef struct DLL _tagKeyInfo
{
	vector<unsigned int>	vecKey;
	string	strKey;
	bool	bPress;
	bool	bPush;
	bool	bUp;
}KEYINFO, *pKEYINFO;

class DLL CInput
{
private:
	unordered_map<string, pKEYINFO>	m_mapKey;
	pKEYINFO	m_pCreateKey;
	short		m_sWheel;
	POINT		m_tMousePos;
	POINT		m_tMouseMove;
	class CGameObject*	m_pMouseObj;
	class CTransform*	m_pMouseTr;
	class CColliderRay*	m_pRay;
	class CColliderPoint*	m_pMousePoint;

	bool		m_bRenderTarget;
public:
	void SetWheel(short sWheel);
	short GetWheelDir()	const;
	void ClearWheel();
	class CGameObject* GetMouseObj();
	class CColliderRay* GetMouseRay();
	class CColliderPoint* GetMousePoint();
	bool RenderTarget();
public:
	bool Init();
	void Update(float fTime);
	void ComputeRay();

public:
	bool KeyPress(const string& strKey);
	bool KeyPush(const string& strKey);
	bool KeyUp(const string& strKey);
	POINT GetMousePos()	const;
	POINT GetMouseMove()	const;

private:
	pKEYINFO FindKey(const string& strKey);
	void ComputeMouse(bool bStart = false);

public:
	// 종료함수
	template <typename T>
	bool CreateKey(const T& key)
	{
		if (typeid(T) != typeid(char) && typeid(T) != typeid(int))
		{
			char*	pKey = (char*)key;
			m_pCreateKey->strKey = pKey;

			if (FindKey(pKey))
			{
				SAFE_DELETE(m_pCreateKey);
				return false;
			}

			m_mapKey.insert(make_pair(pKey, m_pCreateKey));
		}

		else
			m_pCreateKey->vecKey.push_back((unsigned int)key);

		return true;
	}

	template <typename T, typename ... Types>
	bool CreateKey(const T& key, const Types& ...arg)
	{
		if (!m_pCreateKey)
		{
			m_pCreateKey = new KEYINFO;

			m_pCreateKey->bPress = false;
			m_pCreateKey->bPush = false;
			m_pCreateKey->bUp = false;
		}

		if (typeid(T) != typeid(char) && typeid(T) != typeid(int))
		{
			char*	pKey = (char*)key;
			m_pCreateKey->strKey = pKey;

			if (FindKey(pKey))
			{
				SAFE_DELETE(m_pCreateKey);
				return false;
			}

			m_mapKey.insert(make_pair(pKey, m_pCreateKey));
		}

		else
			m_pCreateKey->vecKey.push_back((unsigned int)key);

		CreateKey(arg...);

		if (m_pCreateKey)
			m_pCreateKey = NULL;

		return true;
	}

	DECLARE_SINGLE(CInput)
};

WOOJUN_END
