// TestDialog.cpp : ���� �����Դϴ�.
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

// CTestDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTestDialog, CDialogEx)

CTestDialog::CTestDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CTestDialog::~CTestDialog()
{
	Safe_Release_VecList(m_vecLight);
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
	ON_EN_CHANGE(IDC_EDIT1_POSY, &CTestDialog::OnEnChangeEdit1Posy)
	ON_EN_CHANGE(IDC_EDIT1_POSZ, &CTestDialog::OnEnChangeEdit1Posz)
END_MESSAGE_MAP()


// CTestDialog �޽��� ó�����Դϴ�.

void CTestDialog::OnChildActivate()
{
	CDialogEx::OnChildActivate();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.	
}


void CTestDialog::OnBnClickedButtonCreateSpot()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	LIGHTINFO	tLightInfo = {};
	tLightInfo.eType = LT_SPOTPARENT;
	tLightInfo.vDiffuse = { 0.0f, 0.0f, 0.0f, 1.f };
	tLightInfo.vAmbient = { 1.0f, 0.5f, 0.0f, 1.f };
	tLightInfo.vSpecular = { 1.0f, 0.5f, 0.0f, 1.f };
	tLightInfo.vAttenuation = DxVector3(1.0f, 0.0f, 0.0f);

	CLayer*	pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject* pLightObject = GET_SINGLE(CSceneMgr)->GetCurScene()->CreateLight("SpotLight", LT_SPOT);

	CTransform* pTransform = pLightObject->GetTransform();
	pTransform->SetWorldPos(40.0f, 10.0f, 15.0f);
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

void CTestDialog::OnBnClickedButtonCreatePoint()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CGameObject* pLightObject = GET_SINGLE(CSceneMgr)->GetCurScene()->CreateLight("PointLight", LT_POINT);

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

	m_vecLight.push_back(pPointLight);
	pPointLight->AddRef();
	SAFE_RELEASE(pPointLight);

	SAFE_RELEASE(pLightObject);
	
	m_LightListBox.AddString(L"PointLight");
}


void CTestDialog::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTestDialog::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}



void CTestDialog::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iCurSel = m_LightListBox.GetCurSel();
	
	CTransform* pTransform = m_vecLight[m_iCurSel]->GetTransform();
	DxVector3 vPos = pTransform->GetWorldPos();
	SAFE_RELEASE(pTransform);

	UpdateData(TRUE);

	m_fPosX = vPos.x;
	m_fPosY = vPos.y;
	m_fPosZ = vPos.z;

	UpdateData(FALSE);
}


void CTestDialog::OnEnChangeEdit1Posx2()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CTransform* pTransform = m_vecLight[m_iCurSel]->GetTransform();
	
	UpdateData(TRUE);

	pTransform->SetWorldPosX(m_fPosX);

	UpdateData(FALSE);
		
	SAFE_RELEASE(pTransform);
}


void CTestDialog::OnEnChangeEdit1Posy()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CTestDialog::OnEnChangeEdit1Posz()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
