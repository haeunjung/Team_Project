#include "EndingScene.h"
#include "Engine_Core.h"
#include "01.Core/Input.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Material.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer2D.h"
#include "../ClientMgr/StageMgr.h"

bool CEndingScene::Init()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	// BGM
	CGameObject* pBGM = CGameObject::Create("BGM");
	m_pSoundPlayer = pBGM->AddComponent<CSoundPlayer>("BGMPlayer");
	m_pSoundPlayer->MyPlaySound("Title.mp3", FMOD_LOOP_NORMAL);

	SAFE_RELEASE(pBGM);

	CGameObject*	pBGObject = CGameObject::Create("BGObject");

	CTransform*		pTransform = pBGObject->GetTransform();
	pTransform->SetWorldPos(640.0f, 360.0f, 0.0f);
	pTransform->SetWorldScale(1280.0f, 720.0f, 1.0f);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pBGObject->AddComponent<CRenderer2D>("BGRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "Ending", L"StartScene/Ending.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pRenderer);

	pUILayer->AddObject(pBGObject);
	SAFE_RELEASE(pBGObject);

	SAFE_RELEASE(pUILayer);

	return true;
}

void CEndingScene::Update(float _fTime)
{
	m_fTime += _fTime;
	if (30.0f <= m_fTime)
	{
		GET_SINGLE(CEngine_Core)->Exit();
	}

	if (KEYPRESS("Return"))
	{
		GET_SINGLE(CEngine_Core)->Exit();
	}
}

CEndingScene::CEndingScene() :
	m_fTime(0.0f)
{
}


CEndingScene::~CEndingScene()
{
	SAFE_RELEASE(m_pSoundPlayer);
}
