#pragma once
#include "../Engine_Header.h"

WOOJUN_BEGIN

//class CKey;
class CGameObject;
class CTransform;
class CColliderRay;
class CColliderPoint;
class DLL CKeyMgr
{
private:
	// 말그대로 키 그자체이다.
	class DLL CKey 
	{
		// 키가 처음 눌렸는지.
		// 키가 계속 눌리고 있는지.
		// 키가 떼어졌을때.
		// 'A', 'B' A와 B를 동시에 누르는키다.
	private:
		vector<unsigned int> m_VecKey;
		bool m_bPush;
		bool m_bPress;
		bool m_bUp;
		bool m_bStayUp;
	public:
		void KeyVecReserve(int _Capa);
		void CheckKeyInsert(unsigned int _Key);
		void KeyCheck();
		bool GetAsyncCheck() const;
	public:
		inline bool GetPush()	const { return m_bPush; }
		inline bool GetPress()	const { return m_bPress; }
		inline bool GetUp()		const { return m_bUp; }
		inline bool GetStayUp() const { return m_bStayUp; }
	public:
		CKey();
		~CKey();
	};
private:
	unordered_map<string, CKey*> m_mapKey;
	CKey*	m_pCurKey;
	short	m_sWheel;
	POINT	m_tMousePos;
	POINT	m_tMouseMove;

	CGameObject*	m_pMouseObject;
	CTransform*		m_pMouseTransform;
	CColliderPoint*	m_pColPoint;
	CColliderRay*	m_pColRay;
public:
	void SetWheel(short sWheel);
	short GetWheelDir() const;
	void ClearWheel();
public:
	CGameObject* GetMouseObject();
	CColliderPoint* GetMouseColPoint();
	CColliderRay* GetMouseColRay();
public:
	template<typename... Rest>
	void CreateKey(const string& _strKey, Rest ... _KeyList)
	{
		CKey* tempFindKey = FindKey(_strKey);

		// 이때. 이미 키가 있다면
		if (nullptr != tempFindKey)
		{
			assert(false);
			return;
		}

		m_pCurKey = nullptr;

		int Count = sizeof...(_KeyList);
		CKey* NewKey = new CKey();
		NewKey->KeyVecReserve(Count);
		m_pCurKey = NewKey;

		PushKey(_KeyList...);

		m_mapKey.insert(make_pair(_strKey, NewKey));
	}
private:
	template<typename T, typename... Rest>
	void PushKey(const T& _Key, Rest... _KeyList)
	{
		if (typeid(int) == typeid(_Key) ||
			typeid(char) == typeid(_Key) ||
			typeid(wchar_t) == typeid(_Key))
		{
			m_pCurKey->CheckKeyInsert((unsigned int)_Key);
		}

		PushKey(_KeyList...);
	}
	void PushKey() {
		m_pCurKey = nullptr;
	}
private:
	CKey* FindKey(const string& _strKey) const;
	void ComputeMouse(bool _bStart = false);
public:
	void ComputeRay();
	bool GetPush(const string& _strKey) const;
	bool GetPress(const string& _strKey) const;
	bool GetUp(const string& _strKey) const;
	bool GetStayUp(const string& _strKey) const;
	POINT GetMousePos() const;
	POINT GetMouseMove() const;
public:
	bool Init();
	void Update(float _fTime);

	DECLARE_SINGLE(CKeyMgr)
};

WOOJUN_END