#include "KeyMgr.h"
#include "../Engine_Core.h"
#include "../06.GameObject/GameObject.h"
#include "../07.Component/Transform.h"
#include "../07.Component/ColliderPoint.h"
#include "../07.Component/ColliderRay.h"
#include "CollisionMgr.h"
#include "../05.Scene/Scene.h"
#include "../05.Scene/SceneMgr.h"
#include "../07.Component/Camera.h"

WOOJUN_USING

DEFINITION_SINGLE(CKeyMgr);

///////////////////////////////////////// clsss CKey //////////////////////////
void CKeyMgr::CKey::KeyVecReserve(int _Capa) {
	m_VecKey.reserve(_Capa);
}

void CKeyMgr::CKey::CheckKeyInsert(unsigned int _Key) {
	m_VecKey.push_back(_Key);
}

void CKeyMgr::CKey::KeyCheck() {

	// m_bPush이 문제이다.
	if (true == GetAsyncCheck()) // 키가 제대로 눌려서 true가 반환되었다면
	{
		// 어떤 경우에는 false 어떤 경우에는 저키들이 true
		if (false == m_bPush && false == m_bPress) // 키가 한번도 눌린적이 없다면
		{
			m_bPush = true;
			m_bPress = true;
			m_bUp = false;
			m_bStayUp = false;
		}
		else if (true == m_bPush)
		{
			m_bPush = false;
			m_bPress = true;
			m_bUp = false;
			m_bStayUp = false;
		}
	}
	else 
	{
		// 어떤 경우에는 false 어떤 경우에는 저키들이 true
		if (true == m_bPress) // 키가 한번도 눌린적이 없다면
		{
			m_bPush = false;
			m_bPress = false;
			m_bUp = true;
			m_bStayUp = true;
		}
		else if (true == m_bUp)
		{
			m_bPush = false;
			m_bPress = false;
			m_bUp = false;
			m_bStayUp = true;
		}
	}

}

bool CKeyMgr::CKey::GetAsyncCheck() const {
	size_t VecSize = m_VecKey.size();
	for (size_t i = 0; i < VecSize; ++i)
	{
		if (0 == (GetAsyncKeyState(m_VecKey[i]) & 0x8000))
		{
			return false;
		}
	}

	return true;
}

CKeyMgr::CKey::CKey()
	: m_bPush(false), m_bPress(false), m_bUp(false), m_bStayUp(true)
{

}

CKeyMgr::CKey::~CKey() {
	m_VecKey.clear();
}

///////////////////////////////////////// class CKeyMgr //////////////////////////

bool CKeyMgr::Init()
{
	CreateKey("LButton", VK_LBUTTON);
	CreateKey("RButton", VK_RBUTTON);

	m_sWheel = 0;

	m_pMouseObject = CGameObject::Create("MouseObject");

	m_pMouseTransform = m_pMouseObject->GetTransform();

	m_pRay = m_pMouseObject->AddComponent<CColliderRay>("MouseRay");

	CColliderPoint* pPoint = m_pMouseObject->AddComponent<CColliderPoint>("MousePoint");
	SAFE_RELEASE(pPoint);

	// 마우스 위치를 얻어온다.
	memset(&m_tMousePos, 0, sizeof(POINT));
	memset(&m_tMouseMove, 0, sizeof(POINT));

	ComputeMouse(true);

	return true;
}

void CKeyMgr::Update(float _fTime)
{
	ComputeMouse();

	unordered_map<string, CKey*>::iterator iter = m_mapKey.begin();
	unordered_map<string, CKey*>::iterator iterEnd = m_mapKey.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->KeyCheck();
	}

	m_pMouseObject->Update(_fTime);
	m_pMouseObject->LateUpdate(_fTime);
	GET_SINGLE(CCollisionMgr)->AddObject(m_pMouseObject);
}

void CKeyMgr::SetWheel(short _sWheel)
{
	m_sWheel = _sWheel / 120;
}

short CKeyMgr::GetWheelDir() const
{
	return m_sWheel;
}

void CKeyMgr::ClearWheel()
{
	m_sWheel = 0;
}

CKeyMgr::CKey* CKeyMgr::FindKey(const string& _strKey) const
{
	unordered_map<string, CKey*>::const_iterator iter = m_mapKey.find(_strKey);

	if (m_mapKey.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

void CKeyMgr::ComputeMouse(bool _bStart)
{
	// 클라이언트 크기를 구한다
	RECT	rcClient = {};
	GetClientRect(WINDOWHANDLE, &rcClient);

	float	fX = rcClient.right - rcClient.left;
	float	fY = rcClient.bottom - rcClient.top;

	RESOLUTION	tResolution = _RESOLUTION;

	fX = tResolution.m_iWidth / fX;
	fY = tResolution.m_iHeight / fY;

	POINT	tMousePos;
	GetCursorPos(&tMousePos);
	ScreenToClient(WINDOWHANDLE, &tMousePos);

	tMousePos.x = tMousePos.x * fX;
	tMousePos.y = tMousePos.y * fY;

	if (false == _bStart)
	{
		m_tMouseMove.x = tMousePos.x - m_tMousePos.x;
		m_tMouseMove.y = tMousePos.y - m_tMousePos.y;
	}

	m_tMousePos = tMousePos;

	m_pMouseTransform->SetWorldPos(tMousePos.x, tMousePos.y, 0.0f);

	/*POINT	tMousePos;
	GetCursorPos(&tMousePos);
	ScreenToClient(WINDOWHANDLE, &tMousePos);

	m_tMousePos.x = tMousePos.x - rcClient.right * 0.5f;
	m_tMousePos.y = (tMousePos.y - rcClient.bottom * 0.5f) * -1.0f;*/
}

void CKeyMgr::ComputeRay()
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

bool CKeyMgr::GetPush(const string& _strKey) const
{
	CKey* pKey = FindKey(_strKey);

	assert(pKey);

	return pKey->GetPush();
}
bool CKeyMgr::GetPress(const string& _strKey) const
{
	CKey* pKey = FindKey(_strKey);

	assert(pKey);

	return pKey->GetPress();
}
bool CKeyMgr::GetUp(const string& _strKey) const
{
	CKey* pKey = FindKey(_strKey);

	assert(pKey);

	return pKey->GetUp();
}
bool CKeyMgr::GetStayUp(const string& _strKey) const
{
	CKey* pKey = FindKey(_strKey);

	assert(pKey);

	return pKey->GetStayUp();
}

POINT CKeyMgr::GetMousePos() const
{
	return m_tMousePos;
}

POINT CKeyMgr::GetMouseMove() const
{
	return m_tMouseMove;
}

CKeyMgr::CKeyMgr() 
	: m_pCurKey(NULL)
{
}

CKeyMgr::~CKeyMgr()
{
	SAFE_RELEASE(m_pRay);
	SAFE_RELEASE(m_pMouseObject);
	SAFE_RELEASE(m_pMouseTransform);

	Safe_Delete_Map(m_mapKey);
}