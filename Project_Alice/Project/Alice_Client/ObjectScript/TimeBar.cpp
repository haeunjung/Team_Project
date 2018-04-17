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

bool CTimeBar::Init()
{		
	m_pTransform->SetWorldScale(1000.0f, 30.0f, 1.0f);
	m_pTransform->SetWorldPos(135.0f, 610.0f, 1000.0f);
	//m_pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	CRenderer2D*	pTimeBarRenderer = m_pGameObject->AddComponent<CRenderer2D>("TimeBarRenderer");
	pTimeBarRenderer->SetMesh("UIMesh");
	pTimeBarRenderer->SetShader(UI_SHADER);
	pTimeBarRenderer->SetInputLayout("TexInputLayout");
	pTimeBarRenderer->SetRenderState(ALPHABLEND);	

	CMaterial* pMaterial = pTimeBarRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "TimeBar", L"Striped-Bar-01.png");
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pTimeBarRenderer);

	m_pUIBar = m_pGameObject->AddComponent<CUIBar>("HpBar");
	m_pUIBar->SetMinMax(0.0f, m_fTime);
	m_pUIBar->SetBarDir(BD_LEFT);

	return true;
}

void CTimeBar::Update(float _fTime)
{
	/*if (true == KEYPRESS("F1"))
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
	}*/
}

CTimeBar * CTimeBar::Clone()
{
	return new CTimeBar(*this);
}
