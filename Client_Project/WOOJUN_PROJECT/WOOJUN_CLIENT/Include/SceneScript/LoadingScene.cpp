#include "LoadingScene.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Material.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer2D.h"


bool CLoadingScene::Init()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pBGObject = CGameObject::Create("BGObject");

	CTransform*		pTransform = pBGObject->GetTransform();
	pTransform->SetWorldScale(1280.0f, 720.0f, 1.0f);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pBGObject->AddComponent<CRenderer2D>("BGRenderer2D");
	pRenderer->SetMesh("LTOrthoRect");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "LoadingScene", L"StartScene/Lineage6.bmp");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	pUILayer->AddObject(pBGObject);
	SAFE_RELEASE(pBGObject);

	SAFE_RELEASE(pUILayer);
			
	return true;
}

void CLoadingScene::Update(float _fTime)
{
}

CLoadingScene::CLoadingScene()
{
}


CLoadingScene::~CLoadingScene()
{
}
