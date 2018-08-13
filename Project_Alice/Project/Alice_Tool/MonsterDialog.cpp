// TestDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "MonsterDialog.h"
#include "afxdialogex.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "05.Scene/Layer.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Renderer.h"
#include "ToolValue.h"

// CTestDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMonsterDialog, CDialogEx)

CMonsterDialog::CMonsterDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MONSTER, pParent)
{

}

CMonsterDialog::~CMonsterDialog()
{
	Safe_Release_VecList(m_vecTransform);
}

void CMonsterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_MONSTERPOSX, m_fPosX);
	DDX_Text(pDX, IDC_MONSTERPOSY, m_fPosY);
	DDX_Text(pDX, IDC_MONSTERPOSZ, m_fPosZ);
	
	DDX_Control(pDX, IDC_LIST1, m_MonsterListBox);
}


BEGIN_MESSAGE_MAP(CMonsterDialog, CDialogEx)
	ON_WM_CHILDACTIVATE()
	ON_BN_CLICKED(IDC_SAVEMONSTER, &CMonsterDialog::OnBnClickedSavemonster)
	ON_BN_CLICKED(IDC_LOADMONSTER, &CMonsterDialog::OnBnClickedLoadmonster)
	ON_BN_CLICKED(IDC_CREATEMONSTER, &CMonsterDialog::OnBnClickedCreatemonster)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMonsterDialog::OnLbnSelchangeList1)
	ON_EN_CHANGE(IDC_MONSTERPOSX, &CMonsterDialog::OnEnChangeMonsterposx)
	ON_EN_CHANGE(IDC_MONSTERPOSY, &CMonsterDialog::OnEnChangeMonsterposy)
	ON_EN_CHANGE(IDC_MONSTERPOSZ, &CMonsterDialog::OnEnChangeMonsterposz)
	ON_BN_CLICKED(IDC_CREATEMONSTER2, &CMonsterDialog::OnBnClickedCreatemonster2)
	ON_BN_CLICKED(IDC_CREATEMONSTER3, &CMonsterDialog::OnBnClickedCreatemonster3)
	ON_BN_CLICKED(IDC_DELETEMONSTER, &CMonsterDialog::OnBnClickedDeletemonster)
	ON_BN_CLICKED(IDC_CREATEMONSTER4, &CMonsterDialog::OnBnClickedCreatemonster4)
END_MESSAGE_MAP()


void CMonsterDialog::OnBnClickedSavemonster()
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
		size_t Size = m_vecTransform.size();
		fwrite(&Size, 4, 1, pFile);

		string Tag;
		MONSTER_TYPE Type = MT_DEFAULT;
		for (size_t i = 0; i < Size; ++i)
		{			
			Tag = m_vecTransform[i]->GetTagStr();

			// 이름으로 몬스터 타입 판단 및 저장
			if ("Plant" == Tag)
			{
				Type = MT_PLANT;
				fwrite(&Type, sizeof(MONSTER_TYPE), 1, pFile);
			}
			else if ("Mutant" == Tag)
			{
				Type = MT_MUTANT;
				fwrite(&Type, sizeof(MONSTER_TYPE), 1, pFile);
			}
			else if ("Warrok" == Tag)
			{
				Type = MT_WARROK;
				fwrite(&Type, sizeof(MONSTER_TYPE), 1, pFile);
			}
			else if ("PlantRange" == Tag)
			{
				Type = MT_PLANTRANGE;
				fwrite(&Type, sizeof(MONSTER_TYPE), 1, pFile);
			}

			// WorldPos 저장
			fwrite(&m_vecTransform[i]->GetWorldPos(), sizeof(DxVector3), 1, pFile);
		}

		fclose(pFile);
	}
}


void CMonsterDialog::OnBnClickedLoadmonster()
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

		MONSTER_TYPE Type;
		DxVector3 vPos = {};
		for (size_t i = 0; i < Size; ++i)
		{
			fread(&Type, sizeof(MONSTER_TYPE), 1, pFile);

			// WorldPos 로드		
			fread(&vPos, sizeof(DxVector3), 1, pFile);

			switch (Type)
			{
			case WOOJUN::MT_PLANT:
				CreatePlant(vPos);
				break;
			case WOOJUN::MT_MUTANT:
				CreateMutant(vPos);
				break;
			case WOOJUN::MT_WARROK:
				CreateWarrok(vPos);
				break;
			case WOOJUN::MT_PLANTRANGE:
				CreatePlantRange(vPos);
				break;
			}			
		}

		fclose(pFile);
	}
}

void CMonsterDialog::OnBnClickedCreatemonster()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CreatePlant();
}

void CMonsterDialog::OnBnClickedCreatemonster2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CreateMutant();
}

void CMonsterDialog::OnBnClickedCreatemonster3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CreateWarrok();
}

void CMonsterDialog::OnBnClickedCreatemonster4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CreatePlantRange();
}

void CMonsterDialog::OnBnClickedDeletemonster()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iCurSel = m_MonsterListBox.GetCurSel();

	if (m_iCurSel >= 0)
	{
		vector<CTransform*>::iterator FindIter = find(m_vecTransform.begin(), m_vecTransform.end(), m_vecTransform[m_iCurSel]);

		if (FindIter != m_vecTransform.end())
		{
			CGameObject* pCurObject = m_vecTransform[m_iCurSel]->GetGameObject();

			SAFE_RELEASE((*FindIter));
			m_vecTransform.erase(FindIter);

			pCurObject->Death();
			SAFE_RELEASE(pCurObject);

			m_MonsterListBox.DeleteString(m_iCurSel);
		}			
	}
}

void CMonsterDialog::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iCurSel = m_MonsterListBox.GetCurSel();

	if (m_iCurSel >= 0)
	{		
		DxVector3 vPos = m_vecTransform[m_iCurSel]->GetWorldPos();
		
		UpdateData(TRUE);

		m_fPosX = vPos.x;
		m_fPosY = vPos.y;
		m_fPosZ = vPos.z;

		UpdateData(FALSE);
	}
}


void CMonsterDialog::OnEnChangeMonsterposx()
{
	UpdateData(TRUE);

	m_vecTransform[m_iCurSel]->SetWorldPosX(m_fPosX);

	UpdateData(FALSE);
}


void CMonsterDialog::OnEnChangeMonsterposy()
{
	UpdateData(TRUE);

	m_vecTransform[m_iCurSel]->SetWorldPosY(m_fPosY);

	UpdateData(FALSE);
}


void CMonsterDialog::OnEnChangeMonsterposz()
{
	UpdateData(TRUE);

	m_vecTransform[m_iCurSel]->SetWorldPosZ(m_fPosZ);

	UpdateData(FALSE);
}

void CMonsterDialog::CreatePlant(DxVector3 vPos)
{
	CLayer* pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject*	pMinionObj = CGameObject::Create("Plant");

	CTransform*		pTransform = pMinionObj->GetTransform();
	pTransform->SetTag("Plant");
	pTransform->SetWorldPos(vPos);
	pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	pTransform->SetLocalRotY(-PI * 0.5f);
	pTransform->SetWorldRotY(PI);

	m_vecTransform.push_back(pTransform);
	pTransform->AddRef();
	SAFE_RELEASE(pTransform);

	CRenderer* pPlayerRenderer = pMinionObj->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("Plant", L"SmallMonster.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");
	SAFE_RELEASE(pPlayerRenderer);

	pLayer->AddObject(pMinionObj);
	SAFE_RELEASE(pMinionObj);

	SAFE_RELEASE(pLayer);

	m_MonsterListBox.AddString(L"Plant");
}

void CMonsterDialog::CreatePlantRange(DxVector3 vPos)
{
	CLayer* pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject*	pMinionObj = CGameObject::Create("PlantRange");

	CTransform*		pTransform = pMinionObj->GetTransform();
	pTransform->SetTag("PlantRange");
	pTransform->SetWorldPos(vPos);
	pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	pTransform->SetLocalRotY(-PI * 0.5f);
	pTransform->SetWorldRotY(PI);

	m_vecTransform.push_back(pTransform);
	pTransform->AddRef();
	SAFE_RELEASE(pTransform);

	CRenderer* pPlayerRenderer = pMinionObj->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("Plant", L"SmallMonster.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");
	SAFE_RELEASE(pPlayerRenderer);

	pLayer->AddObject(pMinionObj);
	SAFE_RELEASE(pMinionObj);

	SAFE_RELEASE(pLayer);

	m_MonsterListBox.AddString(L"PlantRange");
}

void CMonsterDialog::CreateMutant(DxVector3 vPos)
{
	CLayer* pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject*	pMinionObj = CGameObject::Create("Mutant");

	CTransform*		pTransform = pMinionObj->GetTransform();
	pTransform->SetTag("Mutant");
	pTransform->SetWorldPos(vPos);
	pTransform->SetWorldScale(0.015f, 0.015f, 0.015f);
	pTransform->SetLocalRotX(-PI * 0.5f);
	//pTransform->SetWorldRotY(PI);

	m_vecTransform.push_back(pTransform);
	pTransform->AddRef();
	SAFE_RELEASE(pTransform);

	CRenderer* pPlayerRenderer = pMinionObj->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("Mutant", L"Mutant.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");
	SAFE_RELEASE(pPlayerRenderer);

	pLayer->AddObject(pMinionObj);
	SAFE_RELEASE(pMinionObj);

	SAFE_RELEASE(pLayer);

	m_MonsterListBox.AddString(L"Mutant");
}

void CMonsterDialog::CreateWarrok(DxVector3 vPos)
{
	CLayer* pLayer = GET_SINGLE(CSceneMgr)->GetCurScene()->FindLayer(DEFAULTLAYER);

	CGameObject*	pMinionObj = CGameObject::Create("Warrok");

	CTransform*		pTransform = pMinionObj->GetTransform();
	pTransform->SetTag("Warrok");
	pTransform->SetWorldPos(vPos);
	pTransform->SetWorldScale(0.012f, 0.012f, 0.012f);
	pTransform->SetLocalRotX(-PI * 0.5f);
	//pTransform->SetWorldRotY(PI);

	m_vecTransform.push_back(pTransform);
	pTransform->AddRef();
	SAFE_RELEASE(pTransform);

	CRenderer* pPlayerRenderer = pMinionObj->AddComponent<CRenderer>("Renderer");
	pPlayerRenderer->SetMesh("Warrok", L"Warrok.msh");
	pPlayerRenderer->SetShader(STANDARD_ANI_BUMP_SHADER);
	pPlayerRenderer->SetInputLayout("AniBumpInputLayout");
	SAFE_RELEASE(pPlayerRenderer);

	pLayer->AddObject(pMinionObj);
	SAFE_RELEASE(pMinionObj);

	SAFE_RELEASE(pLayer);

	m_MonsterListBox.AddString(L"Warrok");
}
