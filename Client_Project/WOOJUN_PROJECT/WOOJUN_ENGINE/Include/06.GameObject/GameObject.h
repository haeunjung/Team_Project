#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class CScene;
class CLayer;
class CTransform;
class CComponent;
class CCollider;
class DLL CGameObject : public CBaseObj
{
private:
	static unordered_map<string, CGameObject*> m_mapPrototype;
public:
	static CGameObject* Create(const string& _strTag, bool _IsPrototype = false);
	static CGameObject* CreateClone(const string& _strKey);
	static void DestroyPrototype();
private:
	static bool CheckProtoType(const string& _strTag);
private:
	CScene*				m_pScene;
	CLayer*				m_pLayer;
	CTransform*			m_pTransform;

	list<CComponent*>		m_listComponent;
	list<CComponent*>::iterator		m_iter;
	list<CComponent*>::iterator		m_iterEnd;
	// 찾은 컴포턴트들을 담아서 리턴할 리스트
	list<CComponent*>		m_listFindComponent;
	// 자식객체
	list<CGameObject*>		m_listChild;
	list<CGameObject*>::iterator	m_Childiter;
	list<CGameObject*>::iterator	m_ChilditerEnd;

	CGameObject*		m_pParent;
public:
	CTransform* GetTransform();
	CGameObject* GetParent();
	CTransform* GetParentTransform();
public:
	void SetScene(CScene* _pScene);
	void SetLayer(CLayer* _pLayer);
	void SetParent(CGameObject* _pParent);
public:
	void ChildTransformActiveUpdate();
	void AddChild(CGameObject* _pChild);
	list<CGameObject*>* GetChildList();
public:
	void AddComponent(CComponent* _pComponent);
	template <typename T>
	T* AddComponent(const string& _strTag)
	{
		T*	pComponent = new T();
		
		// 먼저 세팅하고
		pComponent->SetScene(m_pScene);
		pComponent->SetLayer(m_pLayer);
		pComponent->SetGameObject(this);
		pComponent->SetTransform(m_pTransform);
		
		if (false ==  pComponent->Init())
		{
			SAFE_RELEASE(pComponent);
			return NULL;
		}		
		// 성공하면

		pComponent->SetTag(_strTag);

		// 참조하고
		pComponent->AddRef();
		// 리스트 푸시
		m_listComponent.push_back(pComponent);
		
		return pComponent;
	}
public:
	CComponent* FindComponentFromTag(const string& _strTag);
	CComponent* FindComponentFromTypeName(const string& _strTypeName);
	CComponent* FindComponentFromType(COMPONENT_TYPE _eType);
	template <typename T>
	T* FindComponentFromTypeID()
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_listComponent.end();

		for (iter = m_listComponent.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTypeID() == &typeid(T))
			{
				(*iter)->AddRef();
				return (T*)*iter;
			}
		}
		return NULL;
	}
	
	list<CComponent*>* FindComponentsFromTag(const string& _strTag);
	list<CComponent*>* FindComponentsFromTypeName(const string& _strTypeName);
	list<CComponent*>* FindComponentsFromType(COMPONENT_TYPE _eType);
	template <typename T>
	list<class CComponent*>* FindComponentsFromTypeID()
	{
		//m_listFindComponent.clear();
		Safe_Release_VecList(m_listFindComponent);

		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_listComponent.end();

		for (iter = m_listComponent.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTypeID() == &typeid(T))
			{
				(*iter)->AddRef();
				m_listFindComponent.push_back(*iter);
			}
		}
		return &m_listFindComponent;
	}

	bool CheckComponentFromTag(const string& _strTag);
	bool CheckComponentFromTypeName(const string& _strTypeName);
	bool CheckComponentFromType(COMPONENT_TYPE _eType);
	template <typename T>
	bool CheckComponentFromTypeID()
	{
		list<CComponent*>::iterator	iter;
		list<CComponent*>::iterator	iterEnd = m_listComponent.end();

		for (iter = m_listComponent.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTypeID() == &typeid(T))
			{
				return true;
			}
		}

		return false;
	}
public:
	bool Init();
	void Input(float _fTime);
	void Update(float fTime);
	void LateUpdate(float _fTime);
	void Collision(float _fTime);
	void Render(float _fTime);
	CGameObject* Clone();
public:
	void OnCollisionEnter(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void OnCollisionStay(CCollider* _pSrc, CCollider* _pDest, float _fTime);
	void OnCollisionLeave(CCollider* _pSrc, CCollider* _pDest, float _fTime);
private:
	CGameObject();
	CGameObject(const CGameObject& _GameObject);
	~CGameObject();
};

WOOJUN_END
