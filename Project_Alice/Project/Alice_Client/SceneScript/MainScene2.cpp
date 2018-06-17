#include "MainScene2.h"
#include "../ClientMgr/MinionMgr.h"
#include "../ClientMgr/UIMgr.h"
#include "../ObjectScript/Player.h"
#include "../ObjectScript/Minion.h"
#include "../ObjectScript/Battery.h"
#include "../ObjectScript/Mouse.h"
#include "../ObjectScript/HitEffect.h"
#include "../ObjectScript/PlayerHitEffect.h"
#include "../ObjectScript/MessageBox.h"
#include "../ObjectScript/Portal.h"
#include "01.Core/Input.h"
#include "01.Core/PathMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "05.Scene/SceneMgr.h"
#include "07.Component/Renderer.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Transform.h"
#include "07.Component/Camera.h"
#include "07.Component/CameraArm.h"
#include "07.Component/Material.h"
#include "07.Component/Effect.h"
#include "07.Component/Animation2D.h"
#include "07.Component/ColliderRect.h"
#include "07.Component/UIButton.h"
#include "07.Component/CheckBox.h"
#include "07.Component/RadioButton.h"
#include "07.Component/RadioButtonMgr.h"
#include "07.Component/UIBack.h"
#include "07.Component/Terrain.h"
#include "07.Component/ColliderSphere.h"
#include "07.Component/ColliderAABB.h"
#include "07.Component/SoundPlayer.h"
#include "07.Component/PointLight.h"
#include "07.Component/Light.h"
#include "07.Component/SpotLight.h"
#include "07.Component/ParticleSystem.h"

void CMainScene2::CreateProtoType()
{
	// Battery 
	CGameObject*	pBattery = CGameObject::Create("BatteryObject", true);
	if (pBattery)
	{
		CBattery*	pBattertScript = pBattery->AddComponent<CBattery>("BatteryScript");
		SAFE_RELEASE(pBattertScript);
		SAFE_RELEASE(pBattery);
	}	

	// Effect
	CGameObject*	pEffectObj = CGameObject::Create("HitEffect", true);
	if (pEffectObj)
	{
		CHitEffect*	pEffect = pEffectObj->AddComponent<CHitEffect>("HitEffectScript");
		SAFE_RELEASE(pEffect);
		SAFE_RELEASE(pEffectObj);
	}

	pEffectObj = CGameObject::Create("PlayerHitEffect", true);
	if (pEffectObj)
	{
		CPlayerHitEffect*	pEffect = pEffectObj->AddComponent<CPlayerHitEffect>("PlayerHitEffectScript");
		SAFE_RELEASE(pEffect);
		SAFE_RELEASE(pEffectObj);
	}

	// Particle
	/*CGameObject*	pParticleObj = CGameObject::Create("Particle", true);	
	CParticleSystem*	pParticle = pParticleObj->AddComponent<CParticleSystem>("Particle");
	pParticle->SetParticleInfo();
	pParticle->SetParticleTexture("ParticleSample", L"Effect/PlayerHit2.png");
	pParticle->SetParticleLight(true);
	SAFE_RELEASE(pParticle);
	SAFE_RELEASE(pParticleObj);*/
}

// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

void CMainScene2::CreateObject()
{
	string	strPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(DATAPATH);
	strPath += "Stage2.Data";

	FILE*	pFile = NULL;
	fopen_s(&pFile, strPath.c_str(), "rb");

	if (!pFile)
	{
		return;
	}

	// Vector Size Load
	size_t Size = 0;
	fread(&Size, 4, 1, pFile);
	for (size_t i = 0; i < Size; ++i)
	{
		// 이름 로드
		int iLength = 0;
		fread(&iLength, 4, 1, pFile);
		char strKey[256] = {};
		fread(strKey, 1, iLength, pFile);

		// WorldPos 로드
		DxVector3	vPos;
		fread(&vPos, sizeof(DxVector3), 1, pFile);

		// WorldScale 로드
		DxVector3	vScale;
		fread(&vScale, sizeof(DxVector3), 1, pFile);

		// WorldRot 로드
		DxVector3	vRot;
		fread(&vRot, sizeof(DxVector3), 1, pFile);

		LoadObject(strKey, vPos, vScale, vRot);
	}

	fclose(pFile);
}

void CMainScene2::LoadObject(const string & _strKey, const DxVector3 & _vPos, const DxVector3 & _vScale, const DxVector3 & _vRot)
{
	CLayer*	pLayer = m_pScene->FindLayer(DEFAULTLAYER);

	CGameObject* pGameObject = CGameObject::Create(_strKey);

	CTransform* pTransform = pGameObject->GetTransform();
	pTransform->SetWorldPos(_vPos);
	pTransform->SetWorldScale(_vScale);
	pTransform->SetWorldRot(_vRot);
	SAFE_RELEASE(pTransform);

	/*CToolObject* pToolObject = pGameObject->AddComponent<CToolObject>(_strKey + "Object");
	SAFE_RELEASE(pToolObject);*/

	string	FileName = _strKey + ".FBX";

	CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>(_strKey + "Renderer");
	pRenderer->SetMesh(_strKey, FileName.c_str());
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");

	DxVector3 vMeshSize = pRenderer->GetMeshSize();
	SAFE_RELEASE(pRenderer);

	CColliderAABB* pColAABB = pGameObject->AddComponent<CColliderAABB>(_strKey + "ColAABB");
	pColAABB->SetColCheck(CC_OBJ);
	pColAABB->SetScale(vMeshSize);
	SAFE_RELEASE(pColAABB);

	pLayer->AddObject(pGameObject);

	SAFE_RELEASE(pGameObject);
	SAFE_RELEASE(pLayer);
}

void CMainScene2::CreateCheckBox()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pCheckButtonObject = CGameObject::Create("CheckButtonObject");

	CTransform*	pTransform = pCheckButtonObject->GetTransform();
	DxVector3	vScale = { 100.0f, 100.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(1150.0f, 600.0f, 0.0f);
	pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	CColliderRect*	pColRect = pCheckButtonObject->AddComponent<CColliderRect>("ButtonCol");
	DxVector3	vPos = pTransform->GetWorldPos();
	pColRect->SetRectInfo(0.0f, 0.0f, vScale.x, vScale.y);

	SAFE_RELEASE(pColRect);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pCheckButtonObject->AddComponent<CRenderer2D>("CheckButtonRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "CheckBox", L"free2you2.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CCheckBox*	pCheckBox = pCheckButtonObject->AddComponent<CCheckBox>("CheckBox");
	pCheckBox->SetButtonFunc(this, &CMainScene2::CheckButton);
	SAFE_RELEASE(pCheckBox);

	pUILayer->AddObject(pCheckButtonObject);

	pCheckButtonObject->AddRef();
	m_pCheckBoxObject = pCheckButtonObject;
	m_pCheckBoxObject->SetIsEnable(false);
	
	SAFE_RELEASE(pCheckButtonObject);

	SAFE_RELEASE(pUILayer);
}

void CMainScene2::CreateTerrain()
{
	CLayer*	pMapLayer = m_pScene->FindLayer(MAPLAYER);

	CGameObject*	pTerrainObject = CGameObject::Create("TerrainObject");

	CTransform*		pTransform = pTerrainObject->GetTransform();
	//pTransform->SetWorldPos(-32.0f, -1.0f, -32.0f);
	SAFE_RELEASE(pTransform);

	CTerrain*	pTerrain = pTerrainObject->AddComponent<CTerrain>("Terrain");
	pTerrain->CreateTerrain("Terrain", TERRAINSIZE + 5, TERRAINSIZE, 4, 4/*, "Terrain/Height1.bmp"*/);
	pTerrain->SetBaseTexture("TerrainDiffuse", L"Terrain/TexturesCom_PlywoodNew0050_1_seamless_S_COLOR.png");
	pTerrain->SetNormalTexture("TerrainNormal", L"Terrain/TexturesCom_PlywoodNew0050_1_seamless_S_NRM.png");
	pTerrain->SetSpecularTexture("TerrainSpc", L"Terrain/TexturesCom_PlywoodNew0050_1_seamless_S_SPEC.png");
	SAFE_RELEASE(pTerrain);

	pMapLayer->AddObject(pTerrainObject);
	SAFE_RELEASE(pTerrainObject);
	SAFE_RELEASE(pMapLayer);
}

void CMainScene2::CreateMainSceneLight()
{
	CGameObject* pLightObject = m_pScene->CreateLight("PointLight1", LT_POINT);

	CTransform* pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(15.f, 7.0f, 15.f);
	SAFE_RELEASE(pTransform);

	CPointLight* pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);

	LIGHTINFO	tLightInfo = {};
	tLightInfo.eType = LT_POINT;
	tLightInfo.vDiffuse = { 1.0f, 1.0f, 1.0f, 1.f };
	tLightInfo.vAmbient = { 0.2f, 0.2f, 0.2f, 1.f };
	tLightInfo.vSpecular = { 0.2f, 0.2f, 0.2f, 1.f };
	tLightInfo.vAttenuation = DxVector3(0.0f, 0.2f, 0.0f);

	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("PointLigh2", LT_POINT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(15.f, 7.0f, 50.f);
	SAFE_RELEASE(pTransform);

	pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);
	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("PointLigh3", LT_POINT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(45.f, 10.0f, 27.f);
	SAFE_RELEASE(pTransform);

	pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);
	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("PointLigh4", LT_POINT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(75.0f, 7.0f, 50.f);
	SAFE_RELEASE(pTransform);

	pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);
	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("PointLig5", LT_POINT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(70.0f, 7.0f, 5.f);
	SAFE_RELEASE(pTransform);

	pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);
	pPointLight->SetLightInfo(tLightInfo);
	SAFE_RELEASE(pPointLight);
	SAFE_RELEASE(pLightObject);

	tLightInfo.eType = LT_SPOTPARENT;
	tLightInfo.vDiffuse = { 0.0f, 0.0f, 0.0f, 1.f };
	tLightInfo.vAmbient = { 1.0f, 0.5f, 0.0f, 1.f };
	tLightInfo.vSpecular = { 1.0f, 0.5f, 0.0f, 1.f };
	tLightInfo.vAttenuation = DxVector3(1.0f, 0.0f, 0.0f);

	CLayer* pLayer = m_pScene->FindLayer(DEFAULTLAYER);

	pLightObject = m_pScene->CreateLight("SpotLight1", LT_SPOT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(40.0f, 10.0f, 15.0f);
	SAFE_RELEASE(pTransform);

	CSpotParent* pSpotLight = (CSpotParent*)pLightObject->FindComponentFromType(CT_LIGHT);
	pSpotLight->SetLightInfo(tLightInfo);
	pSpotLight->InitChildSpotLight();
	SAFE_RELEASE(pSpotLight);

	pLayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("SpotLight2", LT_SPOT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(50.0f, 10.0f, 40.0f);
	SAFE_RELEASE(pTransform);

	pSpotLight = (CSpotParent*)pLightObject->FindComponentFromType(CT_LIGHT);
	pSpotLight->SetLightInfo(tLightInfo);
	pSpotLight->InitChildSpotLight();
	SAFE_RELEASE(pSpotLight);

	pLayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("SpotLight3", LT_SPOT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(10.0f, 10.0f, 20.0f);
	SAFE_RELEASE(pTransform);

	pSpotLight = (CSpotParent*)pLightObject->FindComponentFromType(CT_LIGHT);
	pSpotLight->SetLightInfo(tLightInfo);
	pSpotLight->InitChildSpotLight();
	SAFE_RELEASE(pSpotLight);

	pLayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);

	pLightObject = m_pScene->CreateLight("SpotLight4", LT_SPOT);

	pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(77.0f, 10.0f, 20.0f);
	SAFE_RELEASE(pTransform);

	pSpotLight = (CSpotParent*)pLightObject->FindComponentFromType(CT_LIGHT);
	pSpotLight->SetLightInfo(tLightInfo);
	pSpotLight->InitChildSpotLight();
	SAFE_RELEASE(pSpotLight);

	pLayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);

	SAFE_RELEASE(pLayer);
}

void CMainScene2::CreateMonster(CPlayer* _pPlayer)
{
	
}

bool CMainScene2::Init()
{
	CreateProtoType();
	CreateObject();
	CreateTerrain();
	CreateCheckBox();
	//CreateMainSceneLight();

	GET_SINGLE(CUIMgr)->Init(m_pScene);

	CLayer*		pLayer = m_pScene->FindLayer(DEFAULTLAYER);

	// 카메라 오브젝트 생성
	CGameObject*	pCameraObject = m_pScene->GetMainCameraObject();

	CCameraArm*	pCameraArm = pCameraObject->AddComponent<CCameraArm>("CameraArm");
	SAFE_RELEASE(pCameraArm);

	// BGM
	CGameObject* pBGM = CGameObject::Create("BGM");
	CSoundPlayer* pSoundPlayer = pBGM->AddComponent<CSoundPlayer>("BGMPlayer");
	pSoundPlayer->MyPlaySound("Bgm.mp3", FMOD_LOOP_NORMAL);
	SAFE_RELEASE(pSoundPlayer);

	pLayer->AddObject(pBGM);
	SAFE_RELEASE(pBGM);

	// 플레이어
	CGameObject*	pPlayerObject = CGameObject::Create("PlayerObject");
	pLayer->AddObject(pPlayerObject);

	CPlayer*	pPlayerScript = pPlayerObject->AddComponent<CPlayer>("PlayerScript");

	// 몬스터 생성 시 플레이어 정보 넘겨줌
	//CreateMonster(pPlayerScript);

	// TimeBar에 플레이어 정보 넘겨줌
	GET_SINGLE(CUIMgr)->SetPlayer(pPlayerScript);

	SAFE_RELEASE(pPlayerScript);
	SAFE_RELEASE(pPlayerObject);
	SAFE_RELEASE(pCameraObject);

	// Portal
	CGameObject*	pPortalObject = CGameObject::Create("Portal");
	pLayer->AddObject(pPortalObject);

	CPortal*	pPortalScript = pPortalObject->AddComponent<CPortal>("PortalComponent");
	SAFE_RELEASE(pPortalScript);
	SAFE_RELEASE(pPortalObject);

	SAFE_RELEASE(pLayer);

	return true;
}

void CMainScene2::Update(float _fTime)
{
	GET_SINGLE(CMinionMgr)->Update(_fTime);
	GET_SINGLE(CUIMgr)->Update(_fTime);

	if (KEYPRESS("Cheat"))
	{
		m_bEnable = !m_bEnable;
		m_pCheckBoxObject->SetIsEnable(m_bEnable);
	}
}

void CMainScene2::CheckButton(CGameObject * _pObj, float _fTime)
{
	CPlayer::CheatKey();
}

CMainScene2::CMainScene2() :
	m_pCheckBoxObject(NULL),
	m_fRespawnTime(0.0f),
	m_fRespawnLimitTime(10.0f),
	m_bEnable(false)
{
}

CMainScene2::~CMainScene2()
{
	if (false == GET_SINGLE(CSceneMgr)->GetIsChange())
	{
		DESTROY_SINGLE(CMinionMgr);
		DESTROY_SINGLE(CUIMgr);
	}
	SAFE_RELEASE(m_pCheckBoxObject);
}
