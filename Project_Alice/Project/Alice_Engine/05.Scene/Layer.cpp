#include "Layer.h"
#include "Scene.h"
#include "SceneMgr.h"
#include "../03.Resource/Mesh.h"
#include "../04.Rendering/RenderMgr.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/Transform.h"
#include "../07.Component/Frustum.h"
#include "../07.Component/Renderer.h"
#include "../07.Component/Renderer2D.h"

WOOJUN_USING

CScene* CLayer::GetScene() const 
{
	return m_pScene;
}

void CLayer::SetScene(CScene * _pScene)
{
	m_pScene = _pScene;

	list<CGameObject*>::iterator iter = m_listGameObject.begin();
	list<CGameObject*>::iterator iterEnd = m_listGameObject.end();

	for (iter; iter != iterEnd; ++iter)
	{
		(*iter)->SetScene(_pScene);
	}
}

void CLayer::DisableZOrder()
{
	m_bZOrder = false;
}

void CLayer::SetSortFlag(SORT_FLAG _eFlag)
{
	m_eSortFlag = _eFlag;
}

bool CLayer::GetZOrderEnable() const
{
	return m_bZOrder;
}

void CLayer::AddObject(CGameObject * _pGameObject)
{
	_pGameObject->AddRef();
	_pGameObject->SetScene(m_pScene);
	_pGameObject->SetLayer(this);

	m_listGameObject.push_back(_pGameObject);
}

bool CLayer::ObjectZSort(CGameObject * _p1, CGameObject * _p2)
{
	// 두개의 비교대상
	CTransform*	pTransform1 = _p1->GetTransform();
	CTransform*	pTransform2 = _p2->GetTransform();

	// 카메라
	CGameObject*	pCameraObject = GET_SINGLE(CSceneMgr)->GetCurScene()->GetMainCameraObject();
	CTransform*		pCameraTransform = pCameraObject->GetTransform();
	
	// 카메라로부터 거리
	float	fDist1, fDist2 = 0.0f;
	fDist1 = pTransform1->GetWorldPos().Distance(pCameraTransform->GetWorldPos());
	fDist2 = pTransform2->GetWorldPos().Distance(pCameraTransform->GetWorldPos());

	SAFE_RELEASE(pCameraTransform);
	SAFE_RELEASE(pTransform1);
	SAFE_RELEASE(pTransform2);

	SAFE_RELEASE(pCameraObject);

	return fDist1 < fDist2;
}

bool CLayer::ObjectZSortDescending(CGameObject * _p1, CGameObject * _p2)
{
	// 두개의 비교대상
	CTransform*	pTransform1 = _p1->GetTransform();
	CTransform*	pTransform2 = _p2->GetTransform();

	// 카메라
	CGameObject*	pCameraObject = GET_SINGLE(CSceneMgr)->GetCurScene()->GetMainCameraObject();
	CTransform*		pCameraTransform = pCameraObject->GetTransform();

	// 카메라로부터 거리
	float	fDist1, fDist2 = 0.0f;
	fDist1 = pTransform1->GetWorldPos().Distance(pCameraTransform->GetWorldPos());
	fDist2 = pTransform2->GetWorldPos().Distance(pCameraTransform->GetWorldPos());

	SAFE_RELEASE(pCameraTransform);
	SAFE_RELEASE(pTransform1);
	SAFE_RELEASE(pTransform2);

	SAFE_RELEASE(pCameraObject);

	return fDist1 > fDist2;
}

bool CLayer::Init()
{
	return true;
}

void CLayer::Input(float _fTime)
{
	m_iterEnd = m_listGameObject.end();
	for (m_iter = m_listGameObject.begin(); m_iter != m_iterEnd;)
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
			m_iter = m_listGameObject.erase(m_iter);
		}
		else
		{
			++m_iter;
		}
	}
}

void CLayer::Update(float _fTime)
{
	m_iterEnd = m_listGameObject.end();
	for (m_iter = m_listGameObject.begin(); m_iter != m_iterEnd;)
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
			m_iter = m_listGameObject.erase(m_iter);
		}
		else
		{
			++m_iter;
		}
	}
}

void CLayer::LateUpdate(float _fTime)
{
	m_iterEnd = m_listGameObject.end();
	for (m_iter = m_listGameObject.begin(); m_iter != m_iterEnd;)
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
			m_iter = m_listGameObject.erase(m_iter);
		}
		else
		{
			++m_iter;
		}
	}
}

void CLayer::Collision(float _fTime)
{
	m_iterEnd = m_listGameObject.end();
	for (m_iter = m_listGameObject.begin(); m_iter != m_iterEnd;)
	{
		if (false == (*m_iter)->GetIsEnable())
		{
			++m_iter;
			continue;
		}

		(*m_iter)->Collision(_fTime);

		if (false == (*m_iter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_iter));
			m_iter = m_listGameObject.erase(m_iter);
		}
		else
		{
			++m_iter;
		}
	}
}

void CLayer::Render(float _fTime)
{
	/*if (m_listGameObject.size() >= 2)
	{
		switch (m_eSortFlag)
		{
		case SF_ASCENDING:
			m_listGameObject.sort(CLayer::ObjectZSort);
			break;
		case SF_DESCENDING:
			m_listGameObject.sort(CLayer::ObjectZSortDescending);
			break;
		}
	}*/

	m_iterEnd = m_listGameObject.end();
	for (m_iter = m_listGameObject.begin(); m_iter != m_iterEnd;)
	{
		if (false == (*m_iter)->GetIsEnable())
		{
			++m_iter;
			continue;
		}

		CRenderer*		pRenderer = (*m_iter)->FindComponentFromTypeID<CRenderer>();
		CRenderer2D*	pRenderer2D = (*m_iter)->FindComponentFromTypeID<CRenderer2D>();
		CTransform*		pTransform = (*m_iter)->GetTransform();

		bool bFrustum = false;
		CFrustum*		pFrustum = m_pScene->GetMainCameraFrustum();

		if (NULL != pRenderer)
		{
			CMesh*	pMesh = pRenderer->GetMesh();

			SPHEREINFO	tSphereInfo = pMesh->GetSphereInfo();

			DxVector3	vScale = pTransform->GetWorldScale();

			float fMax = vScale.x > vScale.y ? vScale.x : vScale.y;
			fMax = fMax > vScale.z ? fMax : vScale.z;

			tSphereInfo.vCenter *= vScale;
			tSphereInfo.vCenter += pTransform->GetWorldPos();
			tSphereInfo.fRadius *= fMax;

			bFrustum = pFrustum->FrustumInSphere(tSphereInfo);

			SAFE_RELEASE(pMesh);
		}
		else if (pRenderer2D && !(*m_iter)->CheckComponentFromType(CT_UI))
		{
			CMesh*	pMesh = pRenderer2D->GetMesh();

			SPHEREINFO	tSphereInfo = pMesh->GetSphereInfo();

			DxVector3	vScale = pTransform->GetWorldScale();

			float fMax = vScale.x > vScale.y ? vScale.x : vScale.y;
			fMax = fMax > vScale.z ? fMax : vScale.z;

			tSphereInfo.vCenter *= vScale;
			tSphereInfo.vCenter += pTransform->GetWorldPos();
			tSphereInfo.fRadius *= fMax;

			bFrustum = pFrustum->FrustumInSphere(tSphereInfo);

			SAFE_RELEASE(pMesh);
		}

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pRenderer2D);
		SAFE_RELEASE(pFrustum);
		SAFE_RELEASE(pTransform);

		if (false == bFrustum)
		{
			GET_SINGLE(CRenderMgr)->AddRenderObject(*m_iter);
			//(*m_iter)->Render(_fTime);
		}
		else
		{
			int a = 0;
		}

		if (false == (*m_iter)->GetIsAlive())
		{
			SAFE_RELEASE((*m_iter));
			m_iter = m_listGameObject.erase(m_iter);
		}
		else
		{
			++m_iter;
		}
	}
}

CLayer::CLayer() :
	m_iZOrder(0),
	m_bZOrder(false),
	m_eSortFlag(SF_ASCENDING),
	m_pScene(NULL)
{
}

CLayer::~CLayer()
{
	Safe_Release_VecList(m_listGameObject);
}