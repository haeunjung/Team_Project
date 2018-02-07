// ObjectDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "ObjectDialog.h"
#include "afxdialogex.h"

#include "ToolValue.h"
#include "ToolObject.h"
#include "01.Core/PathMgr.h"
#include "03.Resource/ResMgr.h"
#include "03.Resource/Mesh.h"
#include "05.Scene/Scene.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Layer.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"
#include "07.Component/ColliderSphere.h"

// CObjectDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjectDialog, CDialogEx)

CObjectDialog::CObjectDialog(CWnd* pParent /*=NULL*/) :
	CDialogEx(IDD_DIALOG_OBJECT, pParent),
	m_fScaleX(0),
	m_fScaleY(0),
	m_fScaleZ(0),
	m_fRotationX(0),
	m_fRotationY(0),
	m_fRotationZ(0),
	m_fPosX(0),
	m_fPosY(0),
	m_fPosZ(0),
	m_bLoad(false),
	m_iCurIndex(0),
	m_pPickObject(NULL)
{
	m_vecObject.reserve(1000);
}

CObjectDialog::~CObjectDialog()
{
	SAFE_RELEASE(m_pPickObject);
	Safe_Release_VecList(m_vecObject);
}

void CObjectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1_SCALEX, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT1_SCALEY, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT1_SCALEZ, m_fScaleZ);

	DDX_Text(pDX, IDC_EDIT1_ROTATIONX, m_fRotationX);
	DDX_Text(pDX, IDC_EDIT1_ROTATIONY, m_fRotationY);
	DDX_Text(pDX, IDC_EDIT1_ROTATIONZ, m_fRotationZ);

	DDX_Text(pDX, IDC_EDIT1_POSX, m_fPosX);
	DDX_Text(pDX, IDC_EDIT1_POSY, m_fPosY);
	DDX_Text(pDX, IDC_EDIT1_POSZ, m_fPosZ);
	DDX_Control(pDX, IDC_LIST1, m_ObjectListBox);
}


BEGIN_MESSAGE_MAP(CObjectDialog, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjectDialog::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, &CObjectDialog::OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_CREATE_BUTTON, &CObjectDialog::OnBnClickedCreateButton)
	ON_WM_CHILDACTIVATE()
	ON_BN_CLICKED(IDC_SAVEOBJECT, &CObjectDialog::OnBnClickedSaveobject)
	ON_BN_CLICKED(IDC_LOADOBJECT, &CObjectDialog::OnBnClickedLoadobject)
	ON_BN_CLICKED(IDC_ERASEOBJECT, &CObjectDialog::OnBnClickedEraseobject)
END_MESSAGE_MAP()


// CObjectDialog 메시지 처리기입니다.

void CObjectDialog::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;

	CDialogEx::OnCancel();
}


void CObjectDialog::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;

	CDialogEx::OnOK();
}


BOOL CObjectDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (IDC_STATIC_POSX <= nID && nID <= IDC_EDIT1_SCALEZ)
	{
		UpdateData(TRUE);

		if (NULL != m_pPickObject)
		{
			CTransform*		pTransform = m_pPickObject->GetTransform();

			pTransform->SetWorldScale(m_fScaleX, m_fScaleY, m_fScaleZ);
			pTransform->SetWorldRot(m_fRotationX, m_fRotationY, m_fRotationZ);
			pTransform->SetWorldPos(m_fPosX, m_fPosY, m_fPosZ);

			SAFE_RELEASE(pTransform);
		}

		UpdateData(FALSE);		

		SetObjectValue();
	}	

	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CObjectDialog::SetObjectValue()
{
	UpdateData(TRUE);

	if (NULL == m_pPickObject)
	{
		return;
	}

	CTransform*		pTransform = m_pPickObject->GetTransform();

	DxVector3	Scale = pTransform->GetWorldScale();
	m_fScaleX = Scale.x;
	m_fScaleY = Scale.y;
	m_fScaleZ = Scale.z;

	DxVector3	Rotation = pTransform->GetWorldRot();
	m_fRotationX = Rotation.x;
	m_fRotationY = Rotation.y;
	m_fRotationZ = Rotation.z;

	DxVector3	Pos = pTransform->GetWorldPos();
	m_fPosX = Pos.x;
	m_fPosY = Pos.y;
	m_fPosZ = Pos.z;

	SAFE_RELEASE(pTransform);

	UpdateData(FALSE);
}

bool CObjectDialog::LoadFBX()
{
	CFileFind FileFind;

	wstring TempPath = GET_SINGLE(CPathMgr)->FindPath(MESHPATH);
	TempPath += L"*.FBX";
	
	string strKey;

	// 디렉터리가 존재하면 TRUE를 반환한다.
	bool IsFile = FileFind.FindFile(TempPath.c_str());
	while (IsFile)
	{
		// 다음 파일 혹은 폴더가 존재한다면 TRUE를 반환한다.
		IsFile = FileFind.FindNextFileW();

		if (false == FileFind.IsDots())
		{
			CString FileName = FileFind.GetFileName();
			m_ObjectListBox.AddString(FileName);
		}
	}

	return true;
}

void CObjectDialog::CreateObject(const string & _strKey, const wstring & _FileName)
{
	CLayer*	pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject* pGameObject = CGameObject::Create(_strKey);

	CTransform* pTransform = pGameObject->GetTransform();
	pTransform->SetWorldPos(0.0f, 0.0f, 0.0f);
	pTransform->SetWorldScale(0.1f, 0.1f, 0.1f);
	SAFE_RELEASE(pTransform);

	CToolObject* pToolObject = pGameObject->AddComponent<CToolObject>(_strKey + "Object");
	SAFE_RELEASE(pToolObject);
	
	CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>(_strKey + "Renderer");
	pRenderer->SetMesh(_strKey, _FileName.c_str());
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	//pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pRenderer);

	CColliderSphere* pColSphere = pGameObject->AddComponent<CColliderSphere>(_strKey + "ColSphere");
	pColSphere->SetSphereInfo(Vec3Zero, 1.0f);
	SAFE_RELEASE(pColSphere);

	pLayer->AddObject(pGameObject);

	pGameObject->AddRef();
	m_vecObject.push_back(pGameObject);
	SAFE_RELEASE(pGameObject);
	SAFE_RELEASE(pLayer);
}

void CObjectDialog::LoadObject(const string & _strKey, const DxVector3 & _vPos, const DxVector3 & _vScale, const DxVector3 & _vRot)
{
	CLayer*	pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject* pGameObject = CGameObject::Create(_strKey);

	CTransform* pTransform = pGameObject->GetTransform();
	pTransform->SetWorldPos(_vPos);
	pTransform->SetWorldScale(_vScale);
	pTransform->SetWorldRot(_vRot);
	SAFE_RELEASE(pTransform);

	CToolObject* pToolObject = pGameObject->AddComponent<CToolObject>(_strKey + "Object");
	SAFE_RELEASE(pToolObject);

	string	FileName = _strKey + ".FBX";

	CRenderer* pRenderer = pGameObject->AddComponent<CRenderer>(_strKey + "Renderer");
	pRenderer->SetMesh(_strKey, FileName.c_str());
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");
	//pRenderer->SetRenderState(ALPHABLEND);
	SAFE_RELEASE(pRenderer);

	CColliderSphere* pColSphere = pGameObject->AddComponent<CColliderSphere>(_strKey + "ColSphere");
	pColSphere->SetSphereInfo(_vPos, 1.0f);
	SAFE_RELEASE(pColSphere);

	pLayer->AddObject(pGameObject);

	pGameObject->AddRef();
	m_vecObject.push_back(pGameObject);
	SAFE_RELEASE(pGameObject);
	SAFE_RELEASE(pLayer);
}

void CObjectDialog::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iCurIndex = m_ObjectListBox.GetCurSel();
}

void CObjectDialog::ChangePickObject(CGameObject * _pGameObject)
{
	if (NULL == _pGameObject)
	{
		return;
	}

	SAFE_RELEASE(m_pPickObject);
	m_pPickObject = _pGameObject;
	m_pPickObject->AddRef();

	SetObjectValue();
}

void CObjectDialog::OnBnClickedLoadButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (true == m_bLoad)
	{
		return;
	}

	m_bLoad = LoadFBX();
}

void CObjectDialog::OnBnClickedCreateButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	CString GetText;
	m_ObjectListBox.GetText(m_iCurIndex, GetText);
	
	string strKey = GET_SINGLE(CToolValue)->CStringToString(GetText);
	
	int Num = 0;
	for (int i = strlen(strKey.c_str()) - 1; i >= 0; --i)
	{
		++Num;
		if ('.' == strKey[i])
		{
			strKey.erase(i, Num);
			break;
		}
	}

	CreateObject(strKey, GetText.GetString());
}

void CObjectDialog::OnChildActivate()
{
	CDialogEx::OnChildActivate();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	GET_SINGLE(CSceneMgr)->ChangeScene(DEFAULTSCENE);
}


void CObjectDialog::OnBnClickedSaveobject()
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
		size_t Size = m_vecObject.size();
		fwrite(&Size, 4, 1, pFile);
		for (size_t i = 0; i < Size; ++i)
		{
			// 이름 저장
			int iLength = m_vecObject[i]->GetTagStr().length();
			fwrite(&iLength, 4, 1, pFile);
			fwrite(m_vecObject[i]->GetTagStr().c_str(), 1, iLength, pFile);

			CTransform*		pTransform = m_vecObject[i]->GetTransform();

			// WorldPos 저장
			fwrite(&pTransform->GetWorldPos(), sizeof(DxVector3), 1, pFile);

			// WorldScale 저장
			fwrite(&pTransform->GetWorldScale(), sizeof(DxVector3), 1, pFile);

			// WorldRot 저장
			fwrite(&pTransform->GetWorldRot(), sizeof(DxVector3), 1, pFile);

			SAFE_RELEASE(pTransform);
		}

		fclose(pFile);
	}
}


void CObjectDialog::OnBnClickedLoadobject()
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
		for (size_t i = 0; i < Size; ++i)
		{
			// 이름 로드
			int iLength = 0;
			fread(&iLength, 4, 1, pFile);
			char strKey[256] = {};
			fread(strKey, 1, iLength, pFile);
						
			// WorldPos 로드
			DxVector3	vPos;
			fread(&vPos, sizeof(DxVector3), 1, pFile);

			// WorldScale 로드
			DxVector3	vScale;
			fread(&vScale, sizeof(DxVector3), 1, pFile);

			// WorldRot 로드
			DxVector3	vRot;
			fread(&vRot, sizeof(DxVector3), 1, pFile);

			LoadObject(strKey, vPos, vScale, vRot);
		}

		fclose(pFile);
	}
}


void CObjectDialog::OnBnClickedEraseobject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (NULL == m_pPickObject)
	{
		return;
	}

	vector<CGameObject*>::iterator	FindIter = find(m_vecObject.begin(), m_vecObject.end(), m_pPickObject);

	if (FindIter == m_vecObject.end())
	{
		return;
	}
	
	SAFE_RELEASE((*FindIter));
	m_vecObject.erase(FindIter);

	m_pPickObject->Death();
	SAFE_RELEASE(m_pPickObject);
}
