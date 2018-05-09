#include "TimeBar.h"
#include "01.Core/Input.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Transform.h"
#include "07.Component/Material.h"
#include "07.Component/UIBack.h"

CTimeBar::CTimeBar() :
	m_pUIBar(NULL),
	m_fTime(120.0f)
{
	SetTag("Spring");
	SetTypeName("CSpring");
	SetTypeID<CTimeBar>();
}

CTimeBar::CTimeBar(const CTimeBar & _Spring) :
	CScript(_Spring)
{
}

CTimeBar::~CTimeBar()
{
	SAFE_RELEASE(m_pUIBar);
}

void CTimeBar::GetTime()
{
	m_fTime += 10;
}

bool CTimeBar::Init()
{	
	DxVector3 vScale = { 1000.0f, 50.0f, 1.0f };
	DxVector3 vPos = { 135.0f, 610.0f, 500.0f };		
	
	m_pTransform->SetWorldScale(vScale);
	m_pTransform->SetWorldPos(vPos);
	m_pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	CRenderer2D* pTimeBarRenderer = m_pGameObject->AddComponent<CRenderer2D>("TimeBarRenderer");
	pTimeBarRenderer->SetMesh("UIMesh");
	pTimeBarRenderer->SetShader(UI_SHADER);
	pTimeBarRenderer->SetInputLayout("TexInputLayout");
	pTimeBarRenderer->SetRenderState(ALPHABLEND);	

	CMaterial* pMaterial = pTimeBarRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "TimeBar2", L"TimeBar2.png");
	pMaterial->SetDiffuseColor(DxVector4(0.5f, 0.5f, 0.5f, 1.0f));
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pTimeBarRenderer);

	m_pUIBar = m_pGameObject->AddComponent<CUIBar>("HpBar");
	m_pUIBar->SetMinMax(0.0f, m_fTime);
	m_pUIBar->SetBarDir(BD_LEFT);

	return true;
}

void CTimeBar::Update(float _fTime)
{
	m_fTime -= _fTime;
	if (m_fTime <= 0.0f)
	{
		//int a = 0;
	}

	m_pUIBar->SetCurValue(m_fTime);
	if (true == KEYPRESS("F1"))
	{
		if (0 < m_fTime)
		{
			m_fTime -= 10;
			m_pUIBar->SetCurValue(m_fTime);
		}
	}
	else if (true == KEYPRESS("F2"))
	{
		if (120.0f > m_fTime)
		{
			m_fTime += 10;
			m_pUIBar->SetCurValue(m_fTime);
		}
	}
}

CTimeBar * CTimeBar::Clone()
{
	return new CTimeBar(*this);
}
