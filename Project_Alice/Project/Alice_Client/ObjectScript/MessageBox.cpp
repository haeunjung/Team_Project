#include "MessageBox.h"
#include "03.Resource/ResMgr.h"
#include "03.Resource/Texture.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Transform.h"
#include "07.Component/UIBack.h"
#include "07.Component/Material.h"
#include "07.Component/UIButton.h"
#include "07.Component/ColliderRect.h"
#include "../SceneScript/StartScene.h"

bool CMessageBox::Init()
{
	CTransform*	pTransform = m_pGameObject->GetTransform();
	DxVector3	vScale = { 372.0f, 100.5f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(1000.0f, 100.0f, 1000.0f);
	//pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = m_pGameObject->AddComponent<CRenderer2D>("CheckButtonRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "InvenBox", L"MessageWindow.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CUIBack*	pUIBack = m_pGameObject->AddComponent<CUIBack>("InvenBox");
	SAFE_RELEASE(pUIBack);

	CreateButton();

	return true;
}

void CMessageBox::Input(float _fTime)
{
}

void CMessageBox::Update(float _fTime)
{
}

CMessageBox * CMessageBox::Clone()
{
	return new CMessageBox(*this);
}

void CMessageBox::CreateButton()
{
	CLayer*		pUILayer = m_pScene->FindLayer("UILayer");

	CGameObject*	pStartButtonObject = CGameObject::Create("StartButtonObject");

	CTransform*	pTransform = pStartButtonObject->GetTransform();
	DxVector3	vScale = { 48.0f, 21.0f, 1.0f };
	pTransform->SetWorldScale(vScale);
	pTransform->SetWorldPos(1140.0f, 118.0f, 0.0f);
	//pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	CColliderRect*	pColRect = pStartButtonObject->AddComponent<CColliderRect>("ButtonCol");
	DxVector3	vPos = pTransform->GetWorldPos();
	DxVector3	vPivot = pTransform->GetPivot();
	DxVector3	vLeft = vPivot * vScale * -1.f;
	pColRect->SetRectInfo(vLeft.x, vLeft.y, vLeft.x + vScale.x, vLeft.y + vScale.y);

	SAFE_RELEASE(pColRect);
	SAFE_RELEASE(pTransform);

	CRenderer2D*	pRenderer = pStartButtonObject->AddComponent<CRenderer2D>("StartButtonRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "MsgBoxOkButton", L"Btn_OkUp.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	CUIButton*	pButton = pStartButtonObject->AddComponent<CUIButton>("Button");
	pButton->SetNormalColor(1.0f, 1.0f, 1.0f, 1.0f);
	pButton->SetMouseOnColor(1.0f, 1.0f, 1.0f, 1.0f);
	pButton->SetClickColor(0.8f, 0.8f, 0.8f, 0.8f);
	pButton->SetButtonFunc(this, &CMessageBox::OkButton);
	SAFE_RELEASE(pButton);

	pUILayer->AddObject(pStartButtonObject);
	SAFE_RELEASE(pStartButtonObject);

	SAFE_RELEASE(pUILayer);
}

void CMessageBox::OkButton(CGameObject * _pObj, float _fTime)
{
	CScene* pScene = GET_SINGLE(CSceneMgr)->CreateScene("MainScene");
	CStartScene* pStartScene = pScene->CreateScript<CStartScene>();
	GET_SINGLE(CSceneMgr)->ReplaceScene(pScene);
}

CMessageBox::CMessageBox()
{
	SetTag("MessageBox");
	SetTypeName("CMessageBox");
	SetTypeID<CMessageBox>();
}

CMessageBox::CMessageBox(const CMessageBox & _MessageBox) :
	CScript(_MessageBox)
{
}

CMessageBox::~CMessageBox()
{
}
