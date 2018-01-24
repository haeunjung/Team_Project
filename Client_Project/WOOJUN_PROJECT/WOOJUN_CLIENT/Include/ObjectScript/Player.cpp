#include "Player.h"
#include "01.Core/Input.h"
#include "01.Core/Debug.h"
#include "05.Scene/Layer.h"
#include "05.Scene/Scene.h"
#include "03.Resource/Mesh.h"
#include "03.Resource/ResMgr.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/Camera.h"
#include "07.Component/ColliderSphere.h"
#include "07.Component/Material.h"
#include "07.Component/Effect.h"
#include "07.Component/Animation2D.h"

#include "RotBullet.h"
#include "PlayerBullet.h"

bool CPlayer::Init()
{		
	CTransform*		pTransform = m_pGameObject->GetTransform();
	pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	pTransform->SetLocalRotY(-PI * 0.5f);
	SAFE_RELEASE(pTransform);

	CRenderer* pPlayerRenderer = m_pGameObject->AddComponent<CRenderer>("Renderer");
	//pPlayerRenderer->SetMesh("TexNormalPyramid");
	/*for (size_t i = 0; i < 100000; ++i)
	{
		pPlayerRenderer->SetMesh("PlayerMesh", L"SmallMonster.FBX");
	}*/
	//pPlayerRenderer->SetMesh("PlayerMesh", L"SmallMonster.FBX");
	pPlayerRenderer->SetMesh("PlayerMesh", L"PlayerMesh.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");	
	pPlayerRenderer->SetRenderState(ALPHABLEND);

	/*CMesh*	pMesh = GET_SINGLE(CResMgr)->FindMesh("PlayerMesh");
	pMesh->Save("PlayerMesh.msh");
	SAFE_RELEASE(pMesh);*/

	//CMaterial* pMaterial = pPlayerRenderer->GetMaterial();
	////pMaterial->SetDiffuseTexture("Linear", "Texture", L"Texture.png");
	//pMaterial->SetDiffuseTexture("Linear", "Zero", L"0.png");
	//SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pPlayerRenderer);

	CGameObject*	pChild = CGameObject::Create("BoostObject");

	pTransform = pChild->GetTransform();
	pTransform->SetLocalRotY(PI * 0.5f);
	pTransform->SetWorldPos(0.0f, -1.0f, 0.0f);
	SAFE_RELEASE(pTransform);

	CRenderer*	pChildRenderer = pChild->AddComponent<CRenderer>("BoostRenderer");
	pChildRenderer->SetMesh("PosMesh");
	pChildRenderer->SetShader(EFFECT_SHADER);
	pChildRenderer->SetInputLayout("PosInputLayout");
	pChildRenderer->SetRenderState(ALPHABLEND);

	SAFE_RELEASE(pChildRenderer);
		
	CEffect*	pEffect = pChild->AddComponent<CEffect>("Effect");
	SAFE_RELEASE(pEffect);

	CAnimation2D*	pAnimation2D = pChild->AddComponent<CAnimation2D>("EffectAnimation2D");

	vector<wstring>	vecExplosion;
	for (int i = 1; i <= 89; ++i)
	{
		TCHAR	strExplosion[MAX_PATH] = {};
		wsprintf(strExplosion, L"Effect/Explosion/160x120/Explosion%d.png", i);

		vecExplosion.push_back(strExplosion);
	}

	pAnimation2D->AddAnimation2DClip("Explosion", A2D_FRAME, AO_LOOP, vecExplosion.size(), 1, 1.0f, 0, 0.0f, "Explosion", 11, &vecExplosion);
	pAnimation2D->Start(false);

	SAFE_RELEASE(pAnimation2D);
	m_pGameObject->AddChild(pChild);
	m_pChild = pChild;
	
	CColliderSphere*	pSphere = m_pGameObject->AddComponent<CColliderSphere>("Player");
	pSphere->SetSphereInfo(Vec3Zero, 0.5f);
	SAFE_RELEASE(pSphere);

	GET_SINGLE(CInput)->CreateKey("MoveUp", 'Q');
	GET_SINGLE(CInput)->CreateKey("MoveDown", 'E');
	GET_SINGLE(CInput)->CreateKey("MoveForward", 'W');
	GET_SINGLE(CInput)->CreateKey("MoveBack", 'S');
	GET_SINGLE(CInput)->CreateKey("RotLeft", 'A');
	GET_SINGLE(CInput)->CreateKey("RotRight", 'D');

	GET_SINGLE(CInput)->CreateKey("Fire", VK_SPACE);
	GET_SINGLE(CInput)->CreateKey("Init", VK_CONTROL, VK_MENU);	
	GET_SINGLE(CInput)->CreateKey("RotFire", 'R');

	return true;
}

void CPlayer::Input(float _fTime)
{
	if (true == KEYPUSH("MoveUp"))
	{
		m_pTransform->Up(m_fSpeed, _fTime);
		m_bUp = true;
	}
	else if (true == KEYUP("MoveUp"))
	{
		m_bUp = false;
	}
	if (true == KEYPUSH("MoveDown"))
	{
		m_pTransform->Up(-m_fSpeed, _fTime);
	}

	if (true == KEYPUSH("MoveForward"))
	{
		m_pTransform->Forward(m_fSpeed, _fTime);
	}
	if (true == KEYPUSH("MoveBack"))
	{
		m_pTransform->Forward(-m_fSpeed, _fTime);
	}
	if (true == KEYPUSH("RotLeft"))
	{
		m_pTransform->RotateY(-PI, _fTime);
	}
	if (true == KEYPUSH("RotRight"))
	{
		m_pTransform->RotateY(PI, _fTime);
	}

	if (true == KEYPRESS("Fire"))
	{	
		// Bullet Object CreateClone
		CGameObject*	pBulletObject = CGameObject::CreateClone("PlayerBulletObject");		
		// Bullet WorldPos Setting
		// GetTransform() : pBulletObject의 Transform을 가져와서
		// 근데 얘가 AddRef를 하네
		// SetWorldPos() : pBulletObject의 WorldPos를 지정
		CTransform*		pTransform = pBulletObject->GetTransform();
		pTransform->SetWorldPos(m_pTransform->GetWorldPos());
		pTransform->SetWorldRot(m_pTransform->GetWorldRot());
		SAFE_RELEASE(pTransform);

		// 현재 Layer에 추가
		m_pLayer->AddObject(pBulletObject);
		SAFE_RELEASE(pBulletObject);
	}

	if (true == KEYPRESS("RotFire"))
	{
		if (true == CRotBullet::m_IsFire)
		{
			return;
		}		

		CGameObject*	pRotBullet = CGameObject::Create("RotBullet");

		CTransform*		pTransform = pRotBullet->GetTransform();
		pTransform->SetWorldPos(m_pTransform->GetWorldPos());
		pTransform->SetWorldRot(m_pTransform->GetWorldRot());
		SAFE_RELEASE(pTransform);

		// 카메라 체인지 하고
		m_pScene->ChangeCamera("BulletCamera");
		// 카메라 컴포넌트 불러와서
		CCamera* pCamera = m_pScene->GetMainCamera();
		// 어태치 시키고
		pCamera->Attach(pRotBullet, DxVector3(0.0f, 0.0f, -5.0f));
		// 릴리즈
		SAFE_RELEASE(pCamera);	

		CRotBullet*	pBullet = pRotBullet->AddComponent<CRotBullet>("RotBullet");
		SAFE_RELEASE(pBullet);		

		// 현재 Layer에 추가
		m_pLayer->AddObject(pRotBullet);
		SAFE_RELEASE(pRotBullet);		
	}

	if (true == KEYPRESS("Init"))
	{
		m_pTransform->SetWorldPos(DxVector3(0.0f, 0.0f, 0.0f));
		m_pTransform->SetWorldRot(DxVector3(0.0f, 0.0f, 0.0f));
	}
}

void CPlayer::Update(float _fTime)
{	
	if (true == m_bUp)
	{
		m_pChild->SetIsEnable(true);
	}
	else
	{
		m_pChild->SetIsEnable(false);
	}

	/*CTransform*	pTransform = m_pGameObject->GetTransform();
	DxVector3	Pos = pTransform->GetWorldPos();

	char strPos[256] = {};
	sprintf_s(strPos, "X : %.2f ,Y : %.2f, Z : %.2f\n", Pos.x, Pos.y, Pos.z);
	CDebug::OutputConsole(strPos);

	SAFE_RELEASE(pTransform);*/
}

void CPlayer::OnCollisionEnter(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if ("MonsterBullet" == _pDest->GetTagStr())
	{
		m_iHp -= 1;

		char strHp[256] = {};
		sprintf_s(strHp, "Hp : %d\n", m_iHp);
		CDebug::OutputConsole(strHp);
	}
}

CPlayer::CPlayer() :
	m_fSpeed(5.0f),
	m_iHp(100),
	m_bUp(false)
{
	SetTypeID<CPlayer>();
}

CPlayer::~CPlayer()
{		
	SAFE_RELEASE(m_pChild);
}
