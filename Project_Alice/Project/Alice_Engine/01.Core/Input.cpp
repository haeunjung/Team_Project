#include "Input.h"
#include "CollisionMgr.h"
#include "../Engine_Core.h"
//#include "../Device.h"
#include "../05.Scene/Scene.h"
#include "../05.Scene/SceneMgr.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/Transform.h"
#include "../07.Component/ColliderPoint.h"
#include "../07.Component/ColliderRay.h"
#include "../07.Component/Camera.h"

WOOJUN_USING

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	m_pCreateKey(NULL),
	m_pMouseTr(NULL),
	m_pRay(NULL),
	m_pMousePoint(NULL)	
{
#ifdef _DEBUG
	m_bRenderTarget = true;
	m_bCollider = true;
#else // !_DEBUG
	m_bRenderTarget = false;	
	m_bCollider = false;
#endif
}

CInput::~CInput()
{
	SAFE_RELEASE(m_pMousePoint);
	SAFE_RELEASE(m_pRay);
	SAFE_RELEASE(m_pMouseObj);
	SAFE_RELEASE(m_pMouseTr);
	Safe_Delete_Map(m_mapKey);
}

void CInput::SetWheel(short sWheel)
{
	m_sWheel = sWheel / 120;
}

short CInput::GetWheelDir() const
{
	return m_sWheel;
}

void CInput::ClearWheel()
{
	m_sWheel = 0;
}

CGameObject * CInput::GetMouseObj()
{
	m_pMouseObj->AddRef();
	return m_pMouseObj;
}

CColliderRay * CInput::GetMouseRay()
{
	m_pRay->AddRef();
	return m_pRay;
}

CColliderPoint * CInput::GetMousePoint()
{
	m_pMousePoint->AddRef();
	return m_pMousePoint;
}

bool CInput::GetIsRenderTarget()
{
	return m_bRenderTarget;
}

bool CInput::GetIsCollider()
{
	return m_bCollider;
}

bool CInput::Init()
{
	CreateKey("LButton", VK_LBUTTON);
	CreateKey("RButton", VK_RBUTTON);

	CreateKey("F1", VK_F1);
	CreateKey("F2", VK_F2);

	CreateKey("UseBattery", 'X');

	CreateKey("RenderTarget", '1');
	CreateKey("Collider", '2');
	CreateKey("Cheat", '3');
	CreateKey("PlayerLight", 'L');

	m_sWheel = 0;

	m_pMouseObj = CGameObject::Create("MouseObj", false);

	m_pMouseTr = m_pMouseObj->GetTransform();

	m_pMousePoint = m_pMouseObj->AddComponent<CColliderPoint>("MousePoint");

	m_pRay = m_pMouseObj->AddComponent<CColliderRay>("MouseRay");

	// 마우스 위치를 얻어온다.
	memset(&m_tMousePos, 0, sizeof(POINT));
	memset(&m_tMouseMove, 0, sizeof(POINT));

	ComputeMouse(true);

	return true;
}

void CInput::Update(float fTime)
{
	ComputeMouse();
	unordered_map<string, pKEYINFO>::iterator	iter;
	unordered_map<string, pKEYINFO>::iterator	iterEnd = m_mapKey.end();

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int		iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000)
				++iPushCount;
		}

		if (iPushCount == iter->second->vecKey.size())
		{
			if (!iter->second->bPress && !iter->second->bPush)
				iter->second->bPress = true;

			else
			{
				iter->second->bPush = true;
				iter->second->bPress = false;
			}
		}

		else
		{
			if (iter->second->bPress || iter->second->bPush)
			{
				iter->second->bPush = false;
				iter->second->bPress = false;
				iter->second->bUp = true;
			}

			else if (iter->second->bUp)
				iter->second->bUp = false;
		}
	}

	m_pMouseObj->Update(fTime);
	m_pMouseObj->LateUpdate(fTime);
	//GET_SINGLE(CCollisionManager)->AddObject(m_pMouseObj);

	if (true == KEYPRESS("RenderTarget"))
	{
		m_bRenderTarget = !m_bRenderTarget;
	}

	if (true == KEYPRESS("Collider"))
	{
		m_bCollider = !m_bCollider;
	}
}

void CInput::ComputeRay()
{
	CScene*	pScene = GET_SINGLE(CSceneMgr)->GetCurScene();

	CCamera*	pCamera = pScene->GetMainCamera();

	MATRIX	tView = pCamera->GetViewMatrix();
	MATRIX	tProj = pCamera->GetProjMatrix();

	// 마우스 위치를 이용해서 뷰공간의 방향을 구해준다.
	DxVector3	vDir;

	vDir.x = (2.f * m_tMousePos.x / _RESOLUTION.m_iWidth - 1) / tProj._11;
	vDir.y = (-2.f * m_tMousePos.y / _RESOLUTION.m_iHeight + 1) / tProj._22;
	vDir.z = 1.f;

	vDir = vDir.Normalize();

	DxVector3	vPos = Vec3Zero;

	// 월드공간으로 변환한다.
	tView = tView.Inverse();

	vDir = vDir.TransformNormal(tView);
	vDir = vDir.Normalize();
	vPos = vPos.TransformCoord(tView);

	m_pRay->SetRay(vPos, vDir);

	SAFE_RELEASE(pCamera);
}

bool CInput::KeyPress(const string & strKey)
{
	pKEYINFO	pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bPress;
}

bool CInput::KeyPush(const string & strKey)
{
	pKEYINFO	pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bPush;
}

bool CInput::KeyUp(const string & strKey)
{
	pKEYINFO	pInfo = FindKey(strKey);

	if (!pInfo)
		return false;

	return pInfo->bUp;
}

POINT CInput::GetMousePos() const
{
	return m_tMousePos;
}

POINT CInput::GetMouseMove() const
{
	return m_tMouseMove;
}

pKEYINFO CInput::FindKey(const string & strKey)
{
	unordered_map<string, pKEYINFO>::iterator	iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}

void CInput::ComputeMouse(bool bStart)
{
	// 윈도우 클라이언트의 크기를 구한다.
	RECT	rcClient = {};
	GetClientRect(WINDOWHANDLE, &rcClient);

	float	x = (float)(rcClient.right - rcClient.left);
	float	y = (float)(rcClient.bottom - rcClient.top);

	RESOLUTION	tRS = _RESOLUTION;

	x = tRS.m_iWidth / x;
	y = tRS.m_iHeight / y;

	POINT	tMousePos;
	GetCursorPos(&tMousePos);
	ScreenToClient(WINDOWHANDLE, &tMousePos);

	tMousePos.x = (LONG)(tMousePos.x * x);
	tMousePos.y = (LONG)(tMousePos.y * y);

	if (!bStart)
	{
		m_tMouseMove.x = tMousePos.x - m_tMousePos.x;
		m_tMouseMove.y = tMousePos.y - m_tMousePos.y;
	}

	m_tMousePos = tMousePos;

	m_pMouseTr->SetWorldPos((float)tMousePos.x, (float)tMousePos.y, 0.f);
}
