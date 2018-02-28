#include "GameObject.h"
#include "../07.Component/Transform.h"
#include "../01.Core/CollisionMgr.h"

WOOJUN_USING

unordered_map<string, CGameObject*> CGameObject::m_mapPrototype;

CGameObject * CGameObject::Create(const string & _strTag, bool _IsPrototype /*= false*/)
{
	CGameObject*	pGameObject = new CGameObject();

	if (false == pGameObject->Init())
	{
		SAFE_RELEASE(pGameObject);
		return NULL;
	}

	pGameObject->SetTag(_strTag);

	// 프로토타입이라면
	if (true == _IsPrototype)
	{
		// 프로토타입 중복검사
		if (false == CheckProtoType(_strTag))
		{
			assert(false);
			return NULL;
		}				
		// 맵에 인서트 할꺼니까 참조카운트++
		pGameObject->AddRef();
		m_mapPrototype.insert(make_pair(_strTag, pGameObject));
	}

	return pGameObject;
}

CGameObject * CGameObject::CreateClone(const string & _strKey)
{
	unordered_map<string, CGameObject*>::iterator iter = m_mapPrototype.find(_strKey);

	if (m_mapPrototype.end() == iter)
	{
		// 없는 Prototype을 복제할라고 한다.
		assert(false);
		return NULL;
	}

	CGameObject*	pGameObject = iter->second->Clone();

	return pGameObject;
}

void CGameObject::DestroyPrototype()
{
	Safe_Release_Map(m_mapPrototype);
}

bool CGameObject::CheckProtoType(const string & _strTag)
{
	unordered_map<string, CGameObject*>::iterator iter = m_mapPrototype.find(_strTag);
	
	if (m_mapPrototype.end() == iter)
	{
		return true;
	}

	return false;
}

CTransform * CGameObject::GetTransform()
{
	m_pTransform->AddRef();
	return m_pTransform;
}

CGameObject * CGameObject::GetParent()
{
	if (NULL != m_pParent)
	{
		m_pParent->AddRef();
	}

	return m_pParent;
}

CTransform * CGameObject::GetParentTransform()
{
	return m_pTransform->GetParentTransform();
}

void CGameObject::SetScene(CScene* _pScene)
{
	m_pScene = _pScene;

	m_pTransform->SetScene(_pScene);

	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (m_iter; m_iter != m_iterEnd; ++m_iter)
	{
		(*m_iter)->SetScene(_pScene);
	}

	m_Childiter = m_listChild.begin();
	m_ChilditerEnd = m_listChild.end();

	for (m_Childiter; m_Childiter != m_ChilditerEnd; ++m_Childiter)
	{
		(*m_Childiter)->SetScene(_pScene);
	}
}

void CGameObject::SetLayer(CLayer* _pLayer)
{
	m_pLayer = _pLayer;

	m_pTransform->SetLayer(_pLayer);

	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (m_iter; m_iter != m_iterEnd; ++m_iter)
	{
		(*m_iter)->SetLayer(_pLayer);
	}

	m_Childiter = m_listChild.begin();
	m_ChilditerEnd = m_listChild.end();

	for (m_Childiter; m_Childiter != m_ChilditerEnd; ++m_Childiter)
	{
		(*m_Childiter)->SetLayer(_pLayer);
	}
}

void CGameObject::SetParent(CGameObject * _pParent)
{
	m_pParent = _pParent;

	CTransform* pTransform = _pParent->GetTransform();
	m_pTransform->SetParentTransform(pTransform);
	SAFE_RELEASE(pTransform);
}

void CGameObject::ChildTransformActiveUpdate()
{
	m_Childiter = m_listChild.begin();
	m_ChilditerEnd = m_listChild.end();

	for (; m_Childiter != m_ChilditerEnd; ++m_Childiter)
	{
		CTransform*	pTransform = (*m_Childiter)->GetTransform();
		pTransform->ActiveUpdate();
		SAFE_RELEASE(pTransform);
	}
}

void CGameObject::AddChild(CGameObject * _pChild)
{
	_pChild->AddRef();
	_pChild->SetParent(this);
	_pChild->SetScene(m_pScene);
	_pChild->SetLayer(m_pLayer);
	
	CTransform* pTransform = _pChild->GetTransform();
	pTransform->ComputeWorldMatrix();

	SAFE_RELEASE(pTransform);

	m_listChild.push_back(_pChild);
}

list<CGameObject*>* CGameObject::GetChildList()
{
	return &m_listChild;
}

void CGameObject::AddComponent(CComponent * _pComponent)
{
	_pComponent->AddRef();
	_pComponent->SetScene(m_pScene);
	_pComponent->SetLayer(m_pLayer);
	_pComponent->SetGameObject(this);
	_pComponent->SetTransform(m_pTransform);

	m_listComponent.push_back(_pComponent);
}

CComponent * CGameObject::FindComponentFromTag(const string & _strTag)
{
	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (; m_iter != m_iterEnd; ++m_iter)
	{
		if ((*m_iter)->GetTag() == _strTag)
		{
			(*m_iter)->AddRef();
			return *m_iter;
		}
	}

	return NULL;
}

CComponent * CGameObject::FindComponentFromTypeName(const string & _strTypeName)
{
	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (; m_iter != m_iterEnd; ++m_iter)
	{
		if ((*m_iter)->GetTypeName() == _strTypeName)
		{
			(*m_iter)->AddRef();
			return *m_iter;
		}
	}

	return NULL;
}

CComponent * CGameObject::FindComponentFromType(COMPONENT_TYPE _eType)
{
	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (; m_iter != m_iterEnd; ++m_iter)
	{
		if ((*m_iter)->GetComponentType() == _eType)
		{
			(*m_iter)->AddRef();
			return *m_iter;
		}
	}

	return NULL;
}

list<CComponent*>* CGameObject::FindComponentsFromTag(const string & _strTag)
{
	//m_listFindComponent.clear();
	Safe_Release_VecList(m_listFindComponent);

	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (; m_iter != m_iterEnd; ++m_iter)
	{
		if ((*m_iter)->GetTag() == _strTag)
		{
			(*m_iter)->AddRef();
			m_listFindComponent.push_back(*m_iter);
		}
	}

	return &m_listFindComponent;
}

list<CComponent*>* CGameObject::FindComponentsFromTypeName(const string & _strTypeName)
{
	//m_listFindComponent.clear();
	Safe_Release_VecList(m_listFindComponent);

	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (; m_iter != m_iterEnd; ++m_iter)
	{
		if ((*m_iter)->GetTypeName() == _strTypeName)
		{
			(*m_iter)->AddRef();
			m_listFindComponent.push_back(*m_iter);
		}
	}

	return &m_listFindComponent;
}

list<CComponent*>* CGameObject::FindComponentsFromType(COMPONENT_TYPE _eType)
{
	//m_listFindComponent.clear();
	Safe_Release_VecList(m_listFindComponent);

	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (; m_iter != m_iterEnd; ++m_iter)
	{
		if ((*m_iter)->GetComponentType() == _eType)
		{
			(*m_iter)->AddRef();
			m_listFindComponent.push_back(*m_iter);
		}
	}

	return &m_listFindComponent;
}

bool CGameObject::CheckComponentFromTag(const string & _strTag)
{
	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (; m_iter != m_iterEnd; ++m_iter)
	{
		if ((*m_iter)->GetTag() == _strTag)
		{
			return true;
		}
	}

	return false;
}

bool CGameObject::CheckComponentFromTypeName(const string & _strTypeName)
{
	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (; m_iter != m_iterEnd; ++m_iter)
	{
		if ((*m_iter)->GetTypeName() == _strTypeName)
		{
			return true;
		}
	}

	return false;
}

bool CGameObject::CheckComponentFromType(COMPONENT_TYPE _eType)
{
	m_iter = m_listComponent.begin();
	m_iterEnd = m_listComponent.end();

	for (; m_iter != m_iterEnd; ++m_iter)
	{
		if ((*m_iter)->GetComponentType() == _eType)
		{
			return true;
		}
	}

	return false;
}

bool CGameObject::Init()
{
	m_pTransform = new CTransform();

	if (false == m_pTransform->Init())
	{
		SAFE_RELEASE(m_pTransform);
		return false;
	}

	m_pTransform->SetGameObject(this);

	return true;
}

void CGameObject::Input(float _fTime)
{
	// 컴포넌트 돌리고
	m_iterEnd = m_listComponent.end();
	for (m_iter = m_listComponent.begin(); m_iter != m_iterEnd;)
	{
		if (false == (*m_iter)->GetIsEnable())
		{
			++m_iter;
			continue;
		}

		(*m_iter)->Input(_fTime);

		if (false == (*m_iter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_iter));
			m_iter = m_listComponent.erase(m_iter);
		}
		else
		{
			++m_iter;
		}
	}

	// 차일드 돌리고
	m_ChilditerEnd = m_listChild.end();
	for (m_Childiter = m_listChild.begin(); m_Childiter != m_ChilditerEnd;)
	{
		if (false == (*m_Childiter)->GetIsEnable())
		{
			++m_Childiter;
			continue;
		}

		(*m_Childiter)->Input(_fTime);

		if (false == (*m_Childiter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_Childiter));
			m_Childiter = m_listChild.erase(m_Childiter);
		}
		else
		{
			++m_Childiter;
		}
	}
}

void CGameObject::Update(float _fTime)
{
	m_pTransform->Update(_fTime);

	m_iterEnd = m_listComponent.end();
	for (m_iter = m_listComponent.begin(); m_iter != m_iterEnd;)
	{
		if (false == (*m_iter)->GetIsEnable())
		{
			++m_iter;
			continue;
		}

		(*m_iter)->Update(_fTime);

		if (false == (*m_iter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_iter));
			m_iter = m_listComponent.erase(m_iter);
		}
		else
		{
			++m_iter;
		}
	}

	// 차일드 돌리고
	m_ChilditerEnd = m_listChild.end();
	for (m_Childiter = m_listChild.begin(); m_Childiter != m_ChilditerEnd;)
	{
		if (false == (*m_Childiter)->GetIsEnable())
		{
			++m_Childiter;
			continue;
		}

		(*m_Childiter)->Update(_fTime);

		if (false == (*m_Childiter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_Childiter));
			m_Childiter = m_listChild.erase(m_Childiter);
		}
		else
		{
			++m_Childiter;
		}
	}
}

void CGameObject::LateUpdate(float _fTime)
{
	m_iterEnd = m_listComponent.end();
	for (m_iter = m_listComponent.begin(); m_iter != m_iterEnd;)
	{
		if (false == (*m_iter)->GetIsEnable())
		{
			++m_iter;
			continue;
		}

		(*m_iter)->LateUpdate(_fTime);

		if (false == (*m_iter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_iter));
			m_iter = m_listComponent.erase(m_iter);
		}
		else
		{
			++m_iter;
		}
	}

	m_pTransform->LateUpdate(_fTime);

	// 차일드 돌리고
	m_ChilditerEnd = m_listChild.end();
	for (m_Childiter = m_listChild.begin(); m_Childiter != m_ChilditerEnd;)
	{
		if (false == (*m_Childiter)->GetIsEnable())
		{
			++m_Childiter;
			continue;
		}

		(*m_Childiter)->LateUpdate(_fTime);

		if (false == (*m_Childiter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_Childiter));
			m_Childiter = m_listChild.erase(m_Childiter);
		}
		else
		{
			++m_Childiter;
		}
	}
}

void CGameObject::Collision(float _fTime)
{
	//m_iterEnd = m_listComponent.end();
	//for (m_iter = m_listComponent.begin(); m_iter != m_iterEnd;)
	//{
	//	if (false == (*m_iter)->GetIsEnable())
	//	{
	//		++m_iter;
	//		continue;
	//	}

	//	(*m_iter)->Collision(_fTime);

	//	if (false == (*m_iter)->GetIsAlive())
	//	{
	//		SAFE_RELEASE((*m_iter));
	//		m_iter = m_listComponent.erase(m_iter);
	//	}
	//	else
	//	{
	//		++m_iter;
	//	}
	//}
	
	// 충돌관리자에 추가
	GET_SINGLE(CCollisionMgr)->AddObject(this);

	// 차일드 돌리고
	m_ChilditerEnd = m_listChild.end();
	for (m_Childiter = m_listChild.begin(); m_Childiter != m_ChilditerEnd;)
	{
		if (false == (*m_Childiter)->GetIsEnable())
		{
			++m_Childiter;
			continue;
		}

		(*m_Childiter)->Collision(_fTime);

		if (false == (*m_Childiter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_Childiter));
			m_Childiter = m_listChild.erase(m_Childiter);
		}
		else
		{
			++m_Childiter;
		}
	}
}

void CGameObject::Render(float _fTime)
{
	m_iterEnd = m_listComponent.end();
	for (m_iter = m_listComponent.begin(); m_iter != m_iterEnd;)
	{
		if (false == (*m_iter)->GetIsEnable())
		{
			++m_iter;
			continue;
		}

		(*m_iter)->Render(_fTime);

		if (false == (*m_iter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_iter));
			m_iter = m_listComponent.erase(m_iter);
		}
		else
		{
			++m_iter;
		}
	}

	// 차일드 돌리고
	m_ChilditerEnd = m_listChild.end();
	for (m_Childiter = m_listChild.begin(); m_Childiter != m_ChilditerEnd;)
	{
		if (false == (*m_Childiter)->GetIsEnable())
		{
			++m_Childiter;
			continue;
		}

		(*m_Childiter)->Render(_fTime);

		if (false == (*m_Childiter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_Childiter));
			m_Childiter = m_listChild.erase(m_Childiter);
		}
		else
		{
			++m_Childiter;
		}
	}
}

CGameObject * CGameObject::Clone()
{
	return new CGameObject(*this);
}

void CGameObject::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	m_iterEnd = m_listComponent.end();

	for (m_iter = m_listComponent.begin(); m_iter != m_iterEnd; ++m_iter)
	{
		(*m_iter)->OnCollisionEnter(_pSrc, _pDest, _fTime);
	}
}

void CGameObject::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	m_iterEnd = m_listComponent.end();

	for (m_iter = m_listComponent.begin(); m_iter != m_iterEnd; ++m_iter)
	{
		(*m_iter)->OnCollisionStay(_pSrc, _pDest, _fTime);
	}
}

void CGameObject::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	m_iterEnd = m_listComponent.end();

	for (m_iter = m_listComponent.begin(); m_iter != m_iterEnd; ++m_iter)
	{
		(*m_iter)->OnCollisionLeave(_pSrc, _pDest, _fTime);
	}
}

CGameObject::CGameObject() :
	m_pScene(NULL),
	m_pLayer(NULL),
	m_pTransform(NULL),
	m_pParent(NULL)
{
	SetTypeName("CGameObj");
	SetTypeID<CGameObject>();
}

CGameObject::CGameObject(const CGameObject & _GameObject)
{
	*this = _GameObject;

	m_pTransform = _GameObject.m_pTransform->Clone();

	m_listComponent.clear();

	list<CComponent*>::const_iterator iter = _GameObject.m_listComponent.begin();
	list<CComponent*>::const_iterator iterEnd = _GameObject.m_listComponent.end();

	for (; iter != iterEnd; ++iter)
	{
		CComponent* pCom = (*iter)->Clone();
		AddComponent(pCom);

		// AddComponent에서 AddRef하니까
		// Release
		SAFE_RELEASE(pCom);
	}

	list<CGameObject*>::const_iterator ChildIter = _GameObject.m_listChild.begin();
	list<CGameObject*>::const_iterator ChildIterEnd = _GameObject.m_listChild.end();

	m_listChild.clear();
	for (; ChildIter != ChildIterEnd; ++ChildIter)
	{
		CGameObject*	pObj = (*ChildIter)->Clone();
		AddChild(pObj);
		SAFE_RELEASE(pObj);
	}

	m_listFindComponent.clear();
}


CGameObject::~CGameObject()
{
	Safe_Release_VecList(m_listChild);
	Safe_Release_VecList(m_listFindComponent);

	// Transform은 하위 클래스니까
	SAFE_RELEASE(m_pTransform);

	Safe_Release_VecList(m_listComponent);	
}