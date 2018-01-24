// AniDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "AniDialog.h"
#include "afxdialogex.h"

#include "ToolValue.h"
#include "ToolObject.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/ColliderSphere.h"

// CAniDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAniDialog, CDialogEx)

CAniDialog::CAniDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ANIMATION, pParent)
	, m_strClipName(_T(""))
	, m_iStartFrame(0)
	, m_iEndFrame(0)
	, m_strDefaultClip(_T(""))
	, m_strCurFBX(_T(""))
	, m_iPosX(0)
{

}

CAniDialog::~CAniDialog()
{
	SAFE_RELEASE(m_pPickObject);
}

void CAniDialog::ChangePickObject(CGameObject * _pGameObject)
{
	if (NULL == _pGameObject)
	{
		return;
	}

	SAFE_RELEASE(m_pPickObject);
	m_pPickObject = _pGameObject;
	m_pPickObject->AddRef();

	string strTag = m_pPickObject->GetTag();
	wstring wstrTag = wstring(strTag.begin(), strTag.end());
	m_strCurFBX.SetString(wstrTag.c_str(), wstrTag.length());

	UpdateData(FALSE);
}

void CAniDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CLIPNAME, m_strClipName);
	DDX_Text(pDX, IDC_EDIT_STARTFRAME, m_iStartFrame);
	DDX_Text(pDX, IDC_EDIT_ENDFRAME, m_iEndFrame);
	DDX_Control(pDX, IDC_COMBO_ANIOPTION, m_ComboOption);
	DDX_Control(pDX, IDC_LIST1, m_ClipList);
	DDX_Text(pDX, IDC_EDIT_DEFAULTCLIP, m_strDefaultClip);
	DDX_Text(pDX, IDC_EDIT_CURFBX, m_strCurFBX);
}


BEGIN_MESSAGE_MAP(CAniDialog, CDialogEx)
	ON_WM_CHILDACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_MODIFYCLIP, &CAniDialog::OnBnClickedButtonModifyclip)
	ON_BN_CLICKED(IDC_BUTTON_ADDCLIP, &CAniDialog::OnBnClickedButtonAddclip)
	ON_BN_CLICKED(IDC_BUTTON_SAVEANI, &CAniDialog::OnBnClickedButtonSaveani)
	ON_BN_CLICKED(IDC_BUTTON_SETDEFAULTCLIP, &CAniDialog::OnBnClickedButtonSetdefaultclip)
	ON_BN_CLICKED(IDC_BUTTON_LOADFBX, &CAniDialog::OnBnClickedButtonLoadfbx)
END_MESSAGE_MAP()


// CAniDialog �޽��� ó�����Դϴ�.

void CAniDialog::CreateAniObject(const string & _strKey, const wstring & _FullPath)
{
	CLayer*	pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject* pGameObject = CGameObject::Create(_strKey);

	CTransform* pTransform = pGameObject->GetTransform();
	pTransform->SetWorldPos(m_iPosX, 0.0f, 0.0f);
	pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	SAFE_RELEASE(pTransform);

	CToolObject* pToolObject = pGameObject->AddComponent<CToolObject>(_strKey + "Object");
	SAFE_RELEASE(pToolObject);

	CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>(_strKey + "Renderer");
	pRenderer->SetMeshFromFullPath(_strKey, _FullPath.c_str());
	pRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pRenderer->SetInputLayout("AniBumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pRenderer);

	CColliderSphere* pColSphere = pGameObject->AddComponent<CColliderSphere>(_strKey + "ColSphere");	
	pColSphere->SetSphereInfo(m_iPosX, 0.0f, 0.0f, 1.0f);
	SAFE_RELEASE(pColSphere);

	ChangePickObject(pGameObject);

	pLayer->AddObject(pGameObject);
	SAFE_RELEASE(pGameObject);
	SAFE_RELEASE(pLayer);

	m_iPosX += 10;
}


void CAniDialog::OnChildActivate()
{
	CDialogEx::OnChildActivate();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	GET_SINGLE(CSceneMgr)->ChangeScene("AnimationScene");
}

void CAniDialog::OnCancel()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return;

	CDialogEx::OnCancel();
}

void CAniDialog::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return;

	CDialogEx::OnOK();
}

void CAniDialog::OnBnClickedButtonAddclip()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CAniDialog::OnBnClickedButtonModifyclip()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CAniDialog::OnBnClickedButtonSaveani()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CAniDialog::OnBnClickedButtonSetdefaultclip()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CAniDialog::OnBnClickedButtonLoadfbx()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WCHAR	strFilter[] = L"FBX (*.FBX)|*.FBX|All Files(*.*)|*.*||";
	CFileDialog	dlg(TRUE, L"FBX", L"", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		CString	strFullPath = dlg.GetPathName();
		string	strFileName = GET_SINGLE(CToolValue)->CStringToString(dlg.GetFileTitle());
		CreateAniObject(strFileName, strFullPath.GetString());
	}
}
