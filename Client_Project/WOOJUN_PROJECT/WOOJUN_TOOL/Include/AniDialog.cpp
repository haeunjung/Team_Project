// AniDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "AniDialog.h"
#include "afxdialogex.h"

#include "ToolValue.h"
#include "ToolObject.h"
#include "03.Resource/Mesh.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/ColliderSphere.h"
#include "07.Component/Animation3D.h"
#include "07.Component/Animation3DClip.h"
#include "07.Component/Renderer.h"

// CAniDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAniDialog, CDialogEx)

CAniDialog::CAniDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ANIMATION, pParent)
	, m_strClipName(_T(""))
	, m_iStartFrame(0)
	, m_iEndFrame(0)
	, m_strDefaultClip(_T(""))
	, m_strCurFBX(_T(""))
	, m_fPosX(0.f)
	, m_fChangeLimitTime(0)
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
	DDX_Text(pDX, IDC_EDIT_CHANGELIMITTIME, m_fChangeLimitTime);
}


BEGIN_MESSAGE_MAP(CAniDialog, CDialogEx)
	ON_WM_CHILDACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_MODIFYCLIP, &CAniDialog::OnBnClickedButtonModifyclip)
	ON_BN_CLICKED(IDC_BUTTON_ADDCLIP, &CAniDialog::OnBnClickedButtonAddclip)
	ON_BN_CLICKED(IDC_BUTTON_SAVEANI, &CAniDialog::OnBnClickedButtonSaveani)
	ON_BN_CLICKED(IDC_BUTTON_SETDEFAULTCLIP, &CAniDialog::OnBnClickedButtonSetdefaultclip)
	ON_BN_CLICKED(IDC_BUTTON_LOADFBX, &CAniDialog::OnBnClickedButtonLoadfbx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAniDialog::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_LOADANI, &CAniDialog::OnBnClickedButtonLoadani)
	ON_BN_CLICKED(IDC_BUTTON_SETCHANGELIMITTIME, &CAniDialog::OnBnClickedButtonSetchangelimittime)
END_MESSAGE_MAP()


// CAniDialog 메시지 처리기입니다.

void CAniDialog::CreateAniObject(const string & _strKey, const wstring & _FullPath, bool _bLoad /*= false*/)
{
	CLayer*	pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject* pGameObject = CGameObject::Create(_strKey);

	CTransform* pTransform = pGameObject->GetTransform();
	pTransform->SetWorldPos(m_fPosX, 0.0f, 0.0f);
	pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	SAFE_RELEASE(pTransform);

	CToolObject* pToolObject = pGameObject->AddComponent<CToolObject>(_strKey + "Object");
	SAFE_RELEASE(pToolObject);

	CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>(_strKey + "Renderer");
	pRenderer->SetMeshFromFullPath(_strKey, _FullPath.c_str());
	pRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pRenderer->SetInputLayout("AniBumpInputLayout");
	pRenderer->SetRenderState(ALPHABLEND);

	if (_bLoad)
	{
		CAnimation3D*	pAnimation3D = (CAnimation3D*)pGameObject->FindComponentFromType(CT_ANIMATION3D);
		unordered_map<string, class CAnimation3DClip*>* pClipMap = pAnimation3D->GetClips();

		unordered_map<string, class CAnimation3DClip*>::iterator	iter;
		unordered_map<string, class CAnimation3DClip*>::iterator	iterEnd = pClipMap->end();

		for (iter = pClipMap->begin(); iter != iterEnd; ++iter)
		{			
			m_ClipList.AddString(CString(iter->first.c_str()));
		}		

		string	strDefaultClip = pAnimation3D->GetDefaultClipName();
		m_strDefaultClip.SetString(CString(strDefaultClip.c_str()));

		SAFE_RELEASE(pAnimation3D);
	}

	SAFE_RELEASE(pRenderer);

	CColliderSphere* pColSphere = pGameObject->AddComponent<CColliderSphere>(_strKey + "ColSphere");	
	pColSphere->SetSphereInfo(m_fPosX, 0.0f, 0.0f, 1.0f);
	SAFE_RELEASE(pColSphere);

	ChangePickObject(pGameObject);

	pLayer->AddObject(pGameObject);
	SAFE_RELEASE(pGameObject);
	SAFE_RELEASE(pLayer);

	m_fPosX += 10;
}


void CAniDialog::OnChildActivate()
{
	CDialogEx::OnChildActivate();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	GET_SINGLE(CSceneMgr)->ChangeScene("AnimationScene");
}

void CAniDialog::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;

	CDialogEx::OnCancel();
}

void CAniDialog::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;

	CDialogEx::OnOK();
}

void CAniDialog::OnBnClickedButtonAddclip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CAnimation3D*	pAnimation3D = (CAnimation3D*)m_pPickObject->FindComponentFromType(CT_ANIMATION3D);

	if (0 == m_ClipList.GetCount())
	{
		pAnimation3D->ClearClip();
	}

	string	strName = GET_SINGLE(CToolValue)->CStringToString(m_strClipName);
	
	pAnimation3D->AddClip(strName, (ANIMATION_OPTION)m_ComboOption.GetCurSel(), m_iStartFrame, m_iEndFrame);
	SAFE_RELEASE(pAnimation3D);

	m_ClipList.AddString(m_strClipName.GetString());
}

void CAniDialog::OnBnClickedButtonModifyclip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int	iCurSel = m_ClipList.GetCurSel();

	if (-1 == iCurSel)
	{
		return;
	}

	UpdateData(TRUE);
	
	CString	CurKey = _T("");
	m_ClipList.GetText(iCurSel, CurKey);	

	string	strCurKey = GET_SINGLE(CToolValue)->CStringToString(CurKey);
	string	strReplaceKey = GET_SINGLE(CToolValue)->CStringToString(m_strClipName);

	CAnimation3D*	pAnimation3D = (CAnimation3D*)m_pPickObject->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeClipInfo(strCurKey, strReplaceKey, (ANIMATION_OPTION)m_ComboOption.GetCurSel(), m_iStartFrame, m_iEndFrame);
	SAFE_RELEASE(pAnimation3D);

	m_ClipList.DeleteString(iCurSel);
	m_ClipList.InsertString(iCurSel, m_strClipName);
}

void CAniDialog::OnBnClickedButtonSaveani()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WCHAR	strFilter[] = L"Mesh (*.msh)|*.msh|All Files(*.*)|*.*||";
	CFileDialog	dlg(FALSE, L"msh", L"", OFN_OVERWRITEPROMPT, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		CString	strFullPath = dlg.GetPathName();

		// 메쉬, 애니메이션 저장
		CRenderer*	pRenderer = (CRenderer*)m_pPickObject->FindComponentFromType(CT_RENDERER);
		CMesh*	pMesh = pRenderer->GetMesh();

		char	strPath[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, strFullPath.GetString(), -1, strPath, strFullPath.GetLength(), 0, 0);

		pMesh->SaveFromFullPath(strPath);
		SAFE_RELEASE(pMesh);
		SAFE_RELEASE(pRenderer);

		CAnimation3D*	pAnimation = (CAnimation3D*)m_pPickObject->FindComponentFromType(CT_ANIMATION3D);

		memset(strPath + (strlen(strPath) - 3), 0, 3);
		strcat_s(strPath, "anm");

		pAnimation->SaveFromFullPath(strPath);

		SAFE_RELEASE(pAnimation);
	}
}

void CAniDialog::OnBnClickedButtonLoadani()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WCHAR	strFilter[] = L"Mesh (*.msh)|*.msh|All Files(*.*)|*.*||";
	CFileDialog	dlg(TRUE, L"Mesh", L"", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		CString	strFullPath = dlg.GetPathName();
		string	strFileName = GET_SINGLE(CToolValue)->CStringToString(dlg.GetFileTitle());
		CreateAniObject(strFileName, strFullPath.GetString(), true);
	}
}

void CAniDialog::OnBnClickedButtonSetdefaultclip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int	iCurSel = m_ClipList.GetCurSel();

	if (-1 == iCurSel)
	{
		return;
	}

	CString	strName;
	m_ClipList.GetText(iCurSel, strName);

	m_strDefaultClip.SetString(strName);

	CAnimation3D*	pAnimation3D = (CAnimation3D*)m_pPickObject->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->SetDefaultClipName(GET_SINGLE(CToolValue)->CStringToString(strName));	
	SAFE_RELEASE(pAnimation3D);
}

void CAniDialog::OnBnClickedButtonLoadfbx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WCHAR	strFilter[] = L"FBX (*.FBX)|*.FBX|All Files(*.*)|*.*||";
	CFileDialog	dlg(TRUE, L"FBX", L"", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		CString	strFullPath = dlg.GetPathName();
		string	strFileName = GET_SINGLE(CToolValue)->CStringToString(dlg.GetFileTitle());
		CreateAniObject(strFileName, strFullPath.GetString());
	}
}


void CAniDialog::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iCurSel = m_ClipList.GetCurSel();

	if (-1 == iCurSel)
	{
		return;
	}

	CString strName;
	m_ClipList.GetText(iCurSel, strName);

	string	strKey = GET_SINGLE(CToolValue)->CStringToString(strName);

	CAnimation3D*	pAnimation3D = (CAnimation3D*)m_pPickObject->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->ChangeClip(strKey);
	m_fChangeLimitTime = pAnimation3D->GetChangeLimitTime();

	CAnimation3DClip*	pClip = pAnimation3D->FindClip(strKey);
	m_strClipName = strName;
	m_iStartFrame = pClip->GetInfo().iStartFrame;
	m_iEndFrame = pClip->GetInfo().iEndFrame;
	ANIMATION_OPTION eOption = pClip->GetInfo().eOption;

	m_ComboOption.SetCurSel((int)eOption);

	SAFE_RELEASE(pClip);
	SAFE_RELEASE(pAnimation3D);

	UpdateData(FALSE);
}


void CAniDialog::OnBnClickedButtonSetchangelimittime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (!m_pPickObject)
	{
		return;
	}

	CAnimation3D*	pAnimation3D = (CAnimation3D*)m_pPickObject->FindComponentFromType(CT_ANIMATION3D);
	pAnimation3D->SetChangeTime(m_fChangeLimitTime);
	SAFE_RELEASE(pAnimation3D);
}
