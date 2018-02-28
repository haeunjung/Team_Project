#include "RotBullet.h"
#include "07.Component/Renderer.h"
#include "07.Component/Transform.h"
#include "07.Component/Camera.h"

#include "05.Scene/Layer.h"
#include "05.Scene/Scene.h"

#include "../SceneScript/MainScene.h"

bool CRotBullet::m_IsFire = false;

bool CRotBullet::Init()
{
	m_pTransform->SetWorldScale(0.5f, 0.5f, 0.5f);

	/*CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	pRenderer->SetMesh("ColorPyramid");
	pRenderer->SetShader("StandardColorShader");
	pRenderer->SetInputLayout("ColorInputLayout");
	SAFE_RELEASE(pRenderer);*/

	CGameObject* pChild = CGameObject::Create("ChildBullet");

	CTransform* pTransform = pChild->GetTransform();
	pTransform->SetWorldPos(0.0f, 0.0f, 1.0f);
	SAFE_RELEASE(pTransform);

	CRenderer*	pChildRenderer = pChild->AddComponent<CRenderer>("ChildRenderer");
	pChildRenderer->SetMesh("ColorPyramid");
	pChildRenderer->SetShader("StandardColorShader");
	pChildRenderer->SetInputLayout("ColorInputLayout");
	SAFE_RELEASE(pChildRenderer);

	m_pGameObject->AddChild(pChild);	
	m_pChild = pChild;
		
	m_IsFire = true;

	return true;
}

void CRotBullet::Update(float _fTime)
{
	m_pTransform->Up(m_fSpeed, _fTime);
	m_pTransform->RotateY(PI * 4, _fTime);

	m_fDist -= m_fSpeed * _fTime;
	if (0.0f >= m_fDist)
	{
		// 야 나 죽을거니까
		// 카메라 컴포넌트 불러와서
		CCamera* pCamera = m_pScene->GetMainCamera();
		CGameObject* pAttachObject = pCamera->GetAttachObject();
		// 릴리즈
		SAFE_RELEASE(pCamera);

		if (true == pAttachObject->CheckComponentFromTypeID<CRotBullet>())
		{
			// 카메라 체인지 하고
			m_pScene->ChangeCamera(MAINCAMERA);
			SAFE_RELEASE(pAttachObject);
		}		

		m_pGameObject->Death();

		m_IsFire = false;
	}
}

CRotBullet * CRotBullet::Clone()
{
	return new CRotBullet(*this);
}

CRotBullet::CRotBullet() :
	m_fSpeed(5.0f),
	m_fDist(5.0f),
	m_pChild(NULL)
{
	SetTypeID<CRotBullet>();	
}

CRotBullet::CRotBullet(const CRotBullet & _RotBullet) :
	CScript(_RotBullet)
{	
	m_fSpeed = _RotBullet.m_fSpeed;
	m_fDist = _RotBullet.m_fDist;
	m_pChild = NULL;
}

CRotBullet::~CRotBullet()
{		
	SAFE_RELEASE(m_pChild);
}
