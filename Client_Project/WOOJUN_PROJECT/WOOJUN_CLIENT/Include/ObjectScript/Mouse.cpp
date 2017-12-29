#include "Mouse.h"
#include "01.Core/KeyMgr.h"
#include "01.Core/Debug.h"
#include "05.Scene/Layer.h"
#include "05.Scene/Scene.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Effect.h"
#include "07.Component/Animation2D.h"
#include "07.Component/ColliderRect.h"
#include "07.Component/ColliderPoint.h"

bool CMouse::Init()
{	
	//CTransform*	pTransform = m_pGameObject->GetTransform();
	//pTransform->SetWorldScale(0.2f, 0.2f, 1.0f);
	//SAFE_RELEASE(pTransform);

	//CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("MouseRenderer");
	//pRenderer->SetMesh("PosMesh");
	//pRenderer->SetShader(EFFECT_SHADER);
	//pRenderer->SetInputLayout("PosInputLayout");
	//pRenderer->SetRenderState(ALPHABLEND);

	//CEffect*	pEffect = m_pGameObject->AddComponent<CEffect>("Effect");
	//SAFE_RELEASE(pEffect);

	//CAnimation2D*	pAnimation2D = m_pGameObject->AddComponent<CAnimation2D>("MouseAnimation2D");
	
	//vector<wstring>	vecMouse;
	//for (int i = 0; i <= 12; ++i)
	//{
	//	TCHAR	strMouse[MAX_PATH] = {};
	//	wsprintf(strMouse, L"Mouse/Normal_%d.png", i);

	//	vecMouse.push_back(strMouse);
	//}

	//pAnimation2D->AddAnimation2DClip("Mouse", A2D_FRAME, A2DO_LOOP, vecMouse.size(), 1, 1.0f, 0, 0.0f, "Mouse", 11, &vecMouse);
	//pAnimation2D->Start(true);

	//SAFE_RELEASE(pAnimation2D);
	//SAFE_RELEASE(pRenderer);

	CTransform*	pTransform = m_pGameObject->GetTransform();
	DxVector3	vScale = { 5.0f, 5.0f, 1.0f };
	pTransform->SetWorldScale(vScale);	
	/*pTransform->SetWorldPos(100.0f, 100.0f, 0.0f);*/

	CColliderRect*	pColRect = m_pGameObject->AddComponent<CColliderRect>("MouseCol");
	DxVector3	vPos = pTransform->GetWorldPos();
	pColRect->SetRectInfo(0.0f, vScale.y, 0.0f, vScale.x, false);
	SAFE_RELEASE(pColRect);

	//CColliderPoint*		pColPoint = m_pGameObject->AddComponent<CColliderPoint>("MouseCol");
	//SAFE_RELEASE(pColPoint);

	SAFE_RELEASE(pTransform);

	return true;
}

void CMouse::Input(float _fTime)
{
}

void CMouse::Update(float _fTime)
{
	CTransform*	pTransform = m_pGameObject->GetTransform();
		
	DxVector3 Pos = { (float)MOUSEPOS.x, (float)MOUSEPOS.y, 0.0f };
	pTransform->SetWorldPos(Pos);

	//char strMousePos[256] = {};
	//sprintf_s(strMousePos, "[ X : %d, Y : %d\n ]", (int)MOUSEPOS.x, (int)MOUSEPOS.y);
	//CDebug::OutputConsole(strMousePos);

	SAFE_RELEASE(pTransform);
}

void CMouse::LateUpdate(float _fTime)
{
}

void CMouse::Collision(float _fTime)
{
}

CMouse * CMouse::Clone()
{
	return new CMouse(*this);
}

void CMouse::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CMouse::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

void CMouse::OnCollisionLeave(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
}

CMouse::CMouse()
{
}

CMouse::CMouse(const CMouse & _Mouse) :
	CScript(_Mouse)
{
}

CMouse::~CMouse()
{
}