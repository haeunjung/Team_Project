#include "CollisionMgr.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/Collider.h"
#include "../07.Component/Camera.h"
#include "../07.Component/Transform.h"
#include "../05.Scene/SceneMgr.h"
#include "../05.Scene/Scene.h"

WOOJUN_USING

DEFINITION_SINGLE(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
	//Safe_Delete_VecList
}

void CCollisionMgr::AddObject(CGameObject * _pGameObject)
{
	if (false == _pGameObject->CheckComponentFromType(CT_COLLIDER))
	{
		return;
	}

	m_vecCollision.push_back(_pGameObject);
}

bool CCollisionMgr::Init()
{
	m_vecCollision.reserve(1000);

	return true;
}

void CCollisionMgr::Collision(float _fTime)
{
	// 사이즈가 2개 미만이면 서로 충돌할 오브젝트가 없다
	if (1 >= m_vecCollision.size())
	{
		m_vecCollision.clear();
		return;
	}

	sort(m_vecCollision.begin(), m_vecCollision.end(), CCollisionMgr::ZSort);

	vector<CGameObject*>::iterator	iter;
	vector<CGameObject*>::iterator	iterEnd = m_vecCollision.end();

	for (iter = m_vecCollision.begin(); iter != iterEnd - 1; ++iter)
	{		
		for (vector<CGameObject*>::iterator iter1 = iter + 1; iter1 != iterEnd; ++iter1)
		{
			CollisionObject(*iter, *iter1, _fTime);
		}
	}

	m_vecCollision.clear();
}

void CCollisionMgr::CollisionObject(CGameObject * _pSrc, CGameObject * _pDest, float _fTime)
{
	list<CComponent*>* pSrcList = _pSrc->FindComponentsFromType(CT_COLLIDER);
	list<CComponent*>* pDestList = _pDest->FindComponentsFromType(CT_COLLIDER);

	list<CComponent*>::iterator iterSrc;
	list<CComponent*>::iterator iterSrcEnd = pSrcList->end();

	list<CComponent*>::iterator iterDest;
	list<CComponent*>::iterator iterDestEnd = pDestList->end();

	for (iterSrc = pSrcList->begin(); iterSrc != iterSrcEnd; ++iterSrc)
	{
		CCollider*	pSrcCol = (CCollider*)*iterSrc;
		for (iterDest = pDestList->begin(); iterDest != iterDestEnd; ++iterDest)
		{
			CCollider*	pDestCol = (CCollider*)*iterDest;

			if (true == pSrcCol->Collision(pDestCol))
			{
				pSrcCol->CollisionEnable();
				pDestCol->CollisionEnable();

				// 처음 충돌
				if (false == pSrcCol->CheckColList(pDestCol))
				{
					pSrcCol->AddColList(pDestCol);
					pDestCol->AddColList(pSrcCol);

					_pSrc->OnCollisionEnter(pSrcCol, pDestCol, _fTime);
					_pDest->OnCollisionEnter(pDestCol, pSrcCol, _fTime);
					//_pDest->OnCollisionEnter(pSrcCol, pDestCol, _fTime);
				}

				// 충돌 중
				else
				{
					_pSrc->OnCollisionStay(pSrcCol, pDestCol, _fTime);
					_pDest->OnCollisionStay(pDestCol, pSrcCol, _fTime);
					//_pDest->OnCollisionStay(pSrcCol, pDestCol, _fTime);
				}
			}
			// 충돌 빠져나감
			else if (true == pSrcCol->CheckColList(pDestCol))
			{
				pSrcCol->EraseColList(pDestCol);
				pDestCol->EraseColList(pSrcCol);

				_pSrc->OnCollisionLeave(pSrcCol, pDestCol, _fTime);
				_pDest->OnCollisionLeave(pDestCol, pSrcCol, _fTime);
				//_pDest->OnCollisionLeave(pSrcCol, pDestCol, _fTime);
			}
		}
	}	
}

bool CCollisionMgr::ZSort(CGameObject * _pSrc, CGameObject * _pDest)
{
	// 두개의 비교대상
	CTransform*	pSrcTransform = _pSrc->GetTransform();
	CTransform*	pDestTransform = _pDest->GetTransform();

	// Camera Transform	
	CTransform*		pCameraTransform = GET_SINGLE(CSceneMgr)->GetCurScene()->GetMainCameraTransform();

	// 카메라로부터 거리
	float	fDist1, fDist2 = 0.0f;
	fDist1 = pSrcTransform->GetWorldPos().Distance(pCameraTransform->GetWorldPos());
	fDist2 = pDestTransform->GetWorldPos().Distance(pCameraTransform->GetWorldPos());

	SAFE_RELEASE(pCameraTransform);
	SAFE_RELEASE(pSrcTransform);
	SAFE_RELEASE(pDestTransform);

	return fDist1 < fDist2;
}
