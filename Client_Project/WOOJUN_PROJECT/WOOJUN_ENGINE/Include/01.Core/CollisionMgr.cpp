#include "CollisionMgr.h"
#include "../01.Core/Input.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/Collider.h"
#include "../07.Component/ColliderRay.h"
#include "../07.Component/ColliderPoint.h"
#include "../07.Component/Transform.h"


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

	if (false == _pGameObject->CheckComponentFromType(CT_UI))
	{
		m_vecCollision.push_back(_pGameObject);
	}
	else
	{
		m_vecUICollision.push_back(_pGameObject);
	}
	
}

bool CCollisionMgr::Init()
{
	m_vecCollision.reserve(1000);
	m_vecUICollision.reserve(1000);

	return true;
}

void CCollisionMgr::Collision(float _fTime)
{
	// 마우스를 이용한 충돌 먼저
	bool	bMouseCollision = false;

	// Mouse Point와 UI Object 충돌체 사이의 충돌처리
	// 충돌했을경우 True를 반환
	bMouseCollision = CollisionMouseUI(_fTime);

	// 충돌하지 않았을 경우
	// Mouse Ray와 Object 충돌체 사이의 충돌처리
	if (true == bMouseCollision)
	{
		int a = 0;
	}
	else
	{
		// 충돌 목록 정렬
		sort(m_vecCollision.begin(), m_vecCollision.end(), CCollisionMgr::ZSort);

		CollisionMouseObject(_fTime);
	}
	
	vector<CGameObject*>::iterator	iter;
	vector<CGameObject*>::iterator	iterEnd;

	if (1 < m_vecUICollision.size())
	{
		iterEnd = m_vecUICollision.end();
		for (iter = m_vecUICollision.begin(); iter != iterEnd - 1; ++iter)
		{
			for (vector<CGameObject*>::iterator iter1 = iter + 1; iter1 != iterEnd; ++iter1)
			{
				CollisionObject(*iter, *iter1, _fTime);
			}
		}
	}

	if (1 < m_vecCollision.size())
	{
		iterEnd = m_vecCollision.end();
		for (iter = m_vecCollision.begin(); iter != iterEnd - 1; ++iter)
		{
			for (vector<CGameObject*>::iterator iter1 = iter + 1; iter1 != iterEnd; ++iter1)
			{
				CollisionObject(*iter, *iter1, _fTime);
			}
		}
	}

	m_vecUICollision.clear();
	m_vecCollision.clear();
}

bool CCollisionMgr::CollisionMouseUI(float _fTime)
{
	vector<CGameObject*>::iterator	iter;
	vector<CGameObject*>::iterator	iterEnd = m_vecUICollision.end();

	CGameObject*	pMouseObject = GET_SINGLE(CInput)->GetMouseObj();
	CColliderPoint*	pMouseColPoint = GET_SINGLE(CInput)->GetMousePoint();

	for (iter = m_vecUICollision.begin(); iter != iterEnd; ++iter)
	{
		list<CComponent*>* pList = (*iter)->FindComponentsFromType(CT_COLLIDER);

		list<CComponent*>::iterator iter1;
		list<CComponent*>::iterator iter1End = pList->end();

		for (iter1 = pList->begin(); iter1 != iter1End; ++iter1)
		{
			CCollider*	pCol = (CCollider*)*iter1;

			if (true == pMouseColPoint->Collision(pCol))
			{
				pMouseColPoint->CollisionEnable();
				pCol->CollisionEnable();

				// 처음 충돌
				if (false == pMouseColPoint->CheckColList(pCol))
				{
					pMouseColPoint->AddColList(pCol);
					pCol->AddColList(pMouseColPoint);
					
					pMouseObject->OnCollisionEnter(pMouseColPoint, pCol, _fTime);
					(*iter)->OnCollisionEnter(pCol, pMouseColPoint, _fTime);
				}

				// 충돌 중
				else
				{
					pMouseObject->OnCollisionStay(pMouseColPoint, pCol, _fTime);
					(*iter)->OnCollisionStay(pCol, pMouseColPoint, _fTime);
				}

				SAFE_RELEASE(pMouseObject);
				SAFE_RELEASE(pMouseColPoint);

				return true;
			}

			// 충돌 빠져나감
			else if (true == pMouseColPoint->CheckColList(pCol))
			{
				pMouseColPoint->EraseColList(pCol);
				pCol->EraseColList(pMouseColPoint);

				pMouseObject->OnCollisionLeave(pMouseColPoint, pCol, _fTime);
				(*iter)->OnCollisionLeave(pCol, pMouseColPoint, _fTime);
			}
		}
	}

	SAFE_RELEASE(pMouseObject);
	SAFE_RELEASE(pMouseColPoint);

	return false;
}

void CCollisionMgr::CollisionMouseObject(float _fTime)
{
	CGameObject*	pMouseObject = GET_SINGLE(CInput)->GetMouseObj();
	CColliderRay*	pMouseColRay = GET_SINGLE(CInput)->GetMouseRay();

	vector<CGameObject*>::iterator	iter;
	vector<CGameObject*>::iterator	iterEnd = m_vecCollision.end();	

	for (iter = m_vecCollision.begin(); iter != iterEnd; ++iter)
	{
		list<CComponent*>* pList = (*iter)->FindComponentsFromType(CT_COLLIDER);

		list<CComponent*>::iterator iter1;
		list<CComponent*>::iterator iter1End = pList->end();

		for (iter1 = pList->begin(); iter1 != iter1End; ++iter1)
		{
			CCollider*	pCol = (CCollider*)*iter1;

			if (true == pMouseColRay->Collision(pCol))
			{
				pMouseColRay->CollisionEnable();
				pCol->CollisionEnable();

				// 처음 충돌
				if (false == pMouseColRay->CheckColList(pCol))
				{
					pMouseColRay->AddColList(pCol);
					pCol->AddColList(pMouseColRay);

					pMouseObject->OnCollisionEnter(pMouseColRay, pCol, _fTime);
					(*iter)->OnCollisionEnter(pCol, pMouseColRay, _fTime);
				}

				// 충돌 중
				else
				{
					pMouseObject->OnCollisionStay(pMouseColRay, pCol, _fTime);
					(*iter)->OnCollisionStay(pCol, pMouseColRay, _fTime);
				}

				SAFE_RELEASE(pMouseObject);
				SAFE_RELEASE(pMouseColRay);

				return;
			}

			// 충돌 빠져나감
			else if (true == pMouseColRay->CheckColList(pCol))
			{
				pMouseColRay->EraseColList(pCol);
				pCol->EraseColList(pMouseColRay);

				pMouseObject->OnCollisionLeave(pMouseColRay, pCol, _fTime);
				(*iter)->OnCollisionLeave(pCol, pMouseColRay, _fTime);
			}
		}
	}

	SAFE_RELEASE(pMouseObject);
	SAFE_RELEASE(pMouseColRay);
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
	CColliderRay*	pRay = GET_SINGLE(CInput)->GetMouseRay();

	// 두개의 비교대상
	CTransform*	pSrcTransform = _pSrc->GetTransform();
	CTransform*	pDestTransform = _pDest->GetTransform();
	
	// 카메라로부터 거리
	float	fDist1, fDist2 = 0.0f;
	fDist1 = pSrcTransform->GetWorldPos().Distance(pRay->GetRay().vPos);
	fDist2 = pDestTransform->GetWorldPos().Distance(pRay->GetRay().vPos);

	SAFE_RELEASE(pSrcTransform);
	SAFE_RELEASE(pDestTransform);
	SAFE_RELEASE(pRay);

	return fDist1 < fDist2;
}
