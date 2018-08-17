#include "GearCount.h"
#include "../ClientMgr/UIMgr.h"
#include "01.Core/Input.h"
#include "03.Resource/ResMgr.h"
#include "03.Resource/Texture.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Transform.h"
#include "07.Component/UIBack.h"

CGearCount::CGearCount() :
	m_pMaterial(NULL),
	m_iCount(0)
{
	SetTag("GearCount");
	SetTypeName("CBatteryCount");
	SetTypeID<CBatteryCount>();
}

CGearCount::CGearCount(const CGearCount & _GearCount) :
	CScript(_GearCount)
{
	SAFE_RELEASE(m_pMaterial);
	m_pMaterial = _GearCount.m_pMaterial;

	m_iCount = _GearCount.m_iCount;
}


CGearCount::~CGearCount()
{
	SAFE_RELEASE(m_pMaterial);
}

void CGearCount::PlusCount()
{
	if (10 > m_iCount)
	{
		++m_iCount;
	}
}

void CGearCount::MinusCount()
{
	if (0 < m_iCount)
	{
		--m_iCount;
	}	
}

int CGearCount::GetCount()
{
	return m_iCount;
}

bool CGearCount::Init()
{
	DxVector3	vScale = { 49.2f, 57.0f, 1.0f };
	m_pTransform->SetWorldScale(vScale);
	m_pTransform->SetWorldPos(260.0f, 50.0f, 0.0f);
	m_pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	CUIBack*	pUI = m_pGameObject->AddComponent<CUIBack>("BatteryCountUI");
	SAFE_RELEASE(pUI);

	CRenderer2D*	pRenderer = m_pGameObject->AddComponent<CRenderer2D>("BatteryCountRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	m_pMaterial = pRenderer->GetMaterial();
	m_pMaterial->SetDiffuseColor(DxVector4(0.8f, 0.8f, 0.8f, 1.0f));
	SAFE_RELEASE(pRenderer);

	return true;
}



void CGearCount::Update(float _fTime)
{
	if (true == KEYPRESS("F3"))
	{
		++m_iCount;
	}

	SetTexture();
}

CGearCount * CGearCount::Clone()
{
	return new CGearCount(*this);
}

void CGearCount::SetTexture()
{
	switch (m_iCount)
	{
	case 0:
		m_strKey = "Zero";
		break;
	case 1:
		m_strKey = "One";
		break;
	case 2:
		m_strKey = "Two";
		break;
	case 3:
		m_strKey = "Three";
		break;
	case 4:
		m_strKey = "Four";
		break;
	case 5:
		m_strKey = "Five";
		break;
	case 6:
		m_strKey = "Six";
		break;
	case 7:
		m_strKey = "Seven";
		break;
	case 8:
		m_strKey = "Eight";
		break;
	case 9:
		m_strKey = "Nine";
		break;
	}

	m_pMaterial->SetDiffuseTexture("Linear", m_strKey);
}
