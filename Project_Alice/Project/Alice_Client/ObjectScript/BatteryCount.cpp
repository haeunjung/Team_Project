#include "BatteryCount.h"
#include "01.Core/Input.h"
#include "03.Resource/ResMgr.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Renderer2D.h"
#include "07.Component/Transform.h"
#include "07.Component/UIBack.h"

CBatteryCount::CBatteryCount() :
	m_pMaterial(NULL),
	m_iCount(0)
{
	SetTag("BatteryCount");
	SetTypeName("CBatteryCount");
	SetTypeID<CBatteryCount>();
}

CBatteryCount::CBatteryCount(const CBatteryCount & _BatteryCount) :
	CScript(_BatteryCount)
{
	SAFE_RELEASE(m_pMaterial);
	m_pMaterial = _BatteryCount.m_pMaterial;

	m_iCount = _BatteryCount.m_iCount;
}


CBatteryCount::~CBatteryCount()
{
	SAFE_RELEASE(m_pMaterial);
}

void CBatteryCount::PlusCount()
{
	if (10 > m_iCount)
	{
		++m_iCount;
	}
}

void CBatteryCount::MinusCount()
{
	if (0 < m_iCount)
	{
		--m_iCount;
	}	
}

bool CBatteryCount::Init()
{
	GET_SINGLE(CResMgr)->LoadTexture("Zero", "0.png");
	GET_SINGLE(CResMgr)->LoadTexture("One", "1.png");
	GET_SINGLE(CResMgr)->LoadTexture("Two", "2.png");
	GET_SINGLE(CResMgr)->LoadTexture("Three", "3.png");
	GET_SINGLE(CResMgr)->LoadTexture("Four", "4.png");
	GET_SINGLE(CResMgr)->LoadTexture("Five", "5.png");
	GET_SINGLE(CResMgr)->LoadTexture("Six", "6.png");
	GET_SINGLE(CResMgr)->LoadTexture("Seven", "7.png");
	GET_SINGLE(CResMgr)->LoadTexture("Eight", "8.png");
	GET_SINGLE(CResMgr)->LoadTexture("Nine", "9.png");

	DxVector3	vScale = { 49.2f, 57.0f, 1.0f };
	m_pTransform->SetWorldScale(vScale);
	m_pTransform->SetWorldPos(300.0f, 40.0f, 0.0f);
	m_pTransform->SetPivot(0.0f, 0.0f, 0.0f);

	CUIBack*	pUI = m_pGameObject->AddComponent<CUIBack>("BatteryCountUI");
	SAFE_RELEASE(pUI);

	CRenderer2D*	pRenderer = m_pGameObject->AddComponent<CRenderer2D>("BatteryCountRenderer2D");
	pRenderer->SetMesh("UIMesh");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("TexInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	m_pMaterial = pRenderer->GetMaterial();
	//m_pMaterial->SetDiffuseTexture()
	m_pMaterial->SetDiffuseColor(DxVector4(0.8f, 0.8f, 0.8f, 1.0f));
	SAFE_RELEASE(pRenderer);

	return true;
}

void CBatteryCount::Update(float _fTime)
{
	if (true == KEYPRESS("F1"))
	{
		if (0 < m_iCount)
		{
			--m_iCount;
		}
	}
	else if (true == KEYPRESS("F2"))
	{
		if (10 > m_iCount)
		{
			++m_iCount;
		}
	}

	SetTexture();
}

CBatteryCount * CBatteryCount::Clone()
{
	return new CBatteryCount(*this);
}

void CBatteryCount::SetTexture()
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
