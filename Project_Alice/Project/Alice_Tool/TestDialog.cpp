// TestDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "TestDialog.h"
#include "afxdialogex.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/PointLight.h"
#include "07.Component/SpotParent.h"
#include "07.Component/SpotLight.h"
#include "ToolValue.h"

// CTestDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTestDialog, CDialogEx)

CTestDialog::CTestDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CTestDialog::~CTestDialog()
{
	Safe_Release_VecList(m_vecLight);
}

void CTestDialog::CreatePointLight(DxVector3 _vPos)
{
	CGameObject* pLightObject = GET_SINGLE(CSceneMgr)->GetCurScene()->CreateLight("PointLight", LT_POINT);

	CTransform* pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(_vPos);
	SAFE_RELEASE(pTransform);

	CPointLight* pPointLight = (CPointLight*)pLightObject->FindComponentFromType(CT_LIGHT);

	LIGHTINFO	tLightInfo = {};
	tLightInfo.eType = LT_POINT;
	tLightInfo.vDiffuse = { 1.0f, 1.0f, 1.0f, 1.f };
	tLightInfo.vAmbient = { 0.2f, 0.2f, 0.2f, 1.f };
	tLightInfo.vSpecular = { 0.2f, 0.2f, 0.2f, 1.f };
	tLightInfo.vAttenuation = DxVector3(0.0f, 0.2f, 0.0f);

	pPointLight->SetLightInfo(tLightInfo);

	m_vecLight.push_back(pPointLight);
	pPointLight->AddRef();
	SAFE_RELEASE(pPointLight);

	SAFE_RELEASE(pLightObject);

	m_LightListBox.AddString(L"PointLight");
}

void CTestDialog::CreateSpotLight(DxVector3 _vPos)
{
	LIGHTINFO	tLightInfo = {};
	tLightInfo.eType = LT_SPOTPARENT;
	tLightInfo.vDiffuse = { 0.0f, 0.0f, 0.0f, 1.f };
	tLightInfo.vAmbient = { 1.0f, 0.5f, 0.0f, 1.f };
	tLightInfo.vSpecular = { 1.0f, 0.5f, 0.0f, 1.f };
	tLightInfo.vAttenuation = DxVector3(1.0f, 0.0f, 0.0f);

	CLayer*	pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject* pLightObject = GET_SINGLE(CSceneMgr)->GetCurScene()->CreateLight("SpotLight", LT_SPOT);

	CTransform* pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(_vPos);
	SAFE_RELEASE(pTransform);

	CSpotParent* pSpotLight = (CSpotParent*)pLightObject->FindComponentFromType(CT_LIGHT);
	pSpotLight->SetLightInfo(tLightInfo);
	pSpotLight->InitChildSpotLight();

	m_vecLight.push_back(pSpotLight);
	pSpotLight->AddRef();
	SAFE_RELEASE(pSpotLight);

	pLayer->AddObject(pLightObject);
	SAFE_RELEASE(pLightObject);

	SAFE_RELEASE(pLayer);

	m_LightListBox.AddString(L"SpotLight");
}

void CTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1_POSX2, m_fPosX);
	DDX_Text(pDX, IDC_EDIT1_POSY2, m_fPosY);
	DDX_Text(pDX, IDC_EDIT1_POSZ2, m_fPosZ);

	DDX_Control(pDX, IDC_LIST1, m_LightListBox);
}


BEGIN_MESSAGE_MAP(CTestDialog, CDialogEx)
	ON_WM_CHILDACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_CREATELIGHT, &CTestDialog::OnBnClickedButtonCreateSpot)
	ON_BN_CLICKED(IDC_BUTTON_SAVELIGHT, &CTestDialog::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CTestDialog::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_CREATELIGHT2, &CTestDialog::OnBnClickedButtonCreatePoint)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTestDialog::OnLbnSelchangeList1)
	ON_EN_CHANGE(IDC_EDIT1_POSX2, &CTestDialog::OnEnChangeEdit1Posx2)
	ON_EN_CHANGE(IDC_EDIT1_POSZ2, &CTestDialog::OnEnChangeEdit1Posz2)
	ON_EN_CHANGE(IDC_EDIT1_POSY2, &CTestDialog::OnEnChangeEdit1Posy2)
END_MESSAGE_MAP()


// CTestDialog 메시지 처리기입니다.

void CTestDialog::OnChildActivate()
{
	CDialogEx::OnChildActivate();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.	
}

void CTestDialog::OnBnClickedButtonCreateSpot()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CreateSpotLight();
}

void CTestDialog::OnBnClickedButtonCreatePoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CreatePointLight();
}

void CTestDialog::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WCHAR	strFilter[] = L"Data (*.Data)|*.Data|All Files(*.*)|*.*||";
	CFileDialog	dlg(FALSE, L"Data", L"", OFN_OVERWRITEPROMPT, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		string	strPath = GET_SINGLE(CToolValue)->CStringToString(dlg.GetPathName());

		FILE*	pFile = NULL;
		fopen_s(&pFile, strPath.c_str(), "wb");

		if (!pFile)
		{
			return;
		}

		// Vector Size Save
		size_t Size = m_vecLight.size();
		fwrite(&Size, 4, 1, pFile);

		LIGHT_TYPE eLightType = LT_END;
		CTransform* pTransform = NULL;
		for (size_t i = 0; i < Size; ++i)
		{
			// 조명 타입 저장
			eLightType = m_vecLight[i]->GetLightInfo().eType;
			fwrite(&eLightType, sizeof(LIGHT_TYPE), 1, pFile);		

			pTransform = m_vecLight[i]->GetTransform();

			// WorldPos 저장
			fwrite(&pTransform->GetWorldPos(), sizeof(DxVector3), 1, pFile);

			SAFE_RELEASE(pTransform);
		}

		fclose(pFile);
	}
}

void CTestDialog::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WCHAR	strFilter[] = L"Data (*.Data)|*.Data|All Files(*.*)|*.*||";
	CFileDialog	dlg(TRUE, L"Data", L"", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		string	strPath = GET_SINGLE(CToolValue)->CStringToString(dlg.GetPathName());

		FILE*	pFile = NULL;
		fopen_s(&pFile, strPath.c_str(), "rb");

		if (!pFile)
		{
			return;
		}

		// Vector Size Load
		size_t Size = 0;
		fread(&Size, 4, 1, pFile);

		LIGHT_TYPE eLightType = LT_END;
		DxVector3 vPos = {};
		for (size_t i = 0; i < Size; ++i)
		{
			// 조명 타입 로드			
			fread(&eLightType, sizeof(LIGHT_TYPE), 1, pFile);
		
			// WorldPos 로드			
			fread(&vPos, sizeof(DxVector3), 1, pFile);

			switch (eLightType)
			{			
			case WOOJUN::LT_POINT:
				CreatePointLight(vPos);
				break;
			case WOOJUN::LT_SPOTPARENT:
				CreateSpotLight(vPos);
				break;
			}
		}

		fclose(pFile);
	}
}

void CTestDialog::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iCurSel = m_LightListBox.GetCurSel();

	if (m_iCurSel > 0)
	{
		CTransform* pTransform = m_vecLight[m_iCurSel]->GetTransform();
		DxVector3 vPos = pTransform->GetWorldPos();
		SAFE_RELEASE(pTransform);

		UpdateData(TRUE);

		m_fPosX = vPos.x;
		m_fPosY = vPos.y;
		m_fPosZ = vPos.z;

		UpdateData(FALSE);
	}	
}

void CTestDialog::OnEnChangeEdit1Posx2()
{
	LIGHT_TYPE eLightType = m_vecLight[m_iCurSel]->GetLightInfo().eType;

	if (eLightType == LT_POINT)
	{
		CTransform* pTransform = m_vecLight[m_iCurSel]->GetTransform();

		UpdateData(TRUE);

		pTransform->SetWorldPosX(m_fPosX);

		UpdateData(FALSE);

		SAFE_RELEASE(pTransform);
	}
	else if (eLightType == LT_SPOTPARENT)
	{
		CTransform* pChildTransform = ((CSpotParent*)m_vecLight[m_iCurSel])->GetChildSpotLight()->GetTransform();		
		CTransform* pTransform = m_vecLight[m_iCurSel]->GetTransform();

		UpdateData(TRUE);

		pChildTransform->SetWorldPosX(m_fPosX);
		pTransform->SetWorldPosX(m_fPosX);

		UpdateData(FALSE);

		SAFE_RELEASE(pChildTransform);
		SAFE_RELEASE(pTransform);
	}	
}

void CTestDialog::OnEnChangeEdit1Posy2()
{
	LIGHT_TYPE eLightType = m_vecLight[m_iCurSel]->GetLightInfo().eType;

	if (eLightType == LT_POINT)
	{
		CTransform* pTransform = m_vecLight[m_iCurSel]->GetTransform();

		UpdateData(TRUE);

		pTransform->SetWorldPosY(m_fPosY);

		UpdateData(FALSE);

		SAFE_RELEASE(pTransform);
	}
	else if (eLightType == LT_SPOTPARENT)
	{
		CTransform* pChildTransform = ((CSpotParent*)m_vecLight[m_iCurSel])->GetChildSpotLight()->GetTransform();
		CTransform* pTransform = m_vecLight[m_iCurSel]->GetTransform();

		UpdateData(TRUE);

		pChildTransform->SetWorldPosY(m_fPosY);
		pTransform->SetWorldPosY(m_fPosY);

		UpdateData(FALSE);

		SAFE_RELEASE(pChildTransform);
		SAFE_RELEASE(pTransform);
	}
}

void CTestDialog::OnEnChangeEdit1Posz2()
{
	LIGHT_TYPE eLightType = m_vecLight[m_iCurSel]->GetLightInfo().eType;

	if (eLightType == LT_POINT)
	{
		CTransform* pTransform = m_vecLight[m_iCurSel]->GetTransform();

		UpdateData(TRUE);

		pTransform->SetWorldPosZ(m_fPosZ);

		UpdateData(FALSE);

		SAFE_RELEASE(pTransform);
	}
	else if (eLightType == LT_SPOTPARENT)
	{
		CTransform* pChildTransform = ((CSpotParent*)m_vecLight[m_iCurSel])->GetChildSpotLight()->GetTransform();
		CTransform* pTransform = m_vecLight[m_iCurSel]->GetTransform();

		UpdateData(TRUE);

		pChildTransform->SetWorldPosZ(m_fPosZ);
		pTransform->SetWorldPosZ(m_fPosZ);

		UpdateData(FALSE);

		SAFE_RELEASE(pChildTransform);
		SAFE_RELEASE(pTransform);
	}
}

