// ObjectDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WOOJUN_TOOL.h"
#include "ObjectDialog.h"
#include "afxdialogex.h"
#include "ToolValue.h"

#include "07.Component/Transform.h"


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
	m_fPosZ(0)
{
}

CObjectDialog::~CObjectDialog()
{
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
}


BEGIN_MESSAGE_MAP(CObjectDialog, CDialogEx)
	ON_WM_PAINT()
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

		if (NULL != GET_SINGLE(CToolValue)->pPlayerObj)
		{
			CTransform*		pTransform = GET_SINGLE(CToolValue)->pPlayerObj->GetTransform();

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
	if (NULL == GET_SINGLE(CToolValue)->pPlayerObj)
	{
		return;
	}

	UpdateData(TRUE);

	CTransform*		pTransform = GET_SINGLE(CToolValue)->pPlayerObj->GetTransform();

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


void CObjectDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	SetObjectValue();
}
