#include "stdafx.h"
#include "ToolObject.h"
#include "ToolValue.h"
#include "ObjectDialog.h"
#include "AniDialog.h"
#include "01.Core/Input.h"
#include "05.Scene/SceneMgr.h"
#include "05.Scene/Scene.h"
#include "07.Component/Collider.h"

bool CToolObject::Init()
{
	return true;
}

void CToolObject::Input(float _fTIme)
{

}

void CToolObject::Update(float _fTime)
{

}

CToolObject * CToolObject::Clone()
{
	return new CToolObject(*this);
}

void CToolObject::OnCollisionStay(CCollider * _pSrc, CCollider * _pDest, float _fTime)
{
	if (true == KEYPRESS("LButton"))
	{
		if ("MouseRay" == _pDest->GetTagStr())
		{
			CheckCurScene();			
		}		
	}	
}

void CToolObject::CheckCurScene()
{
	string	strCurScene = GET_SINGLE(CSceneMgr)->GetCurScene()->GetTag();

	if (DEFAULTSCENE == strCurScene)
	{
		GET_SINGLE(CToolValue)->m_pObjectDialog->ChangePickObject(m_pGameObject);
	}
	else if ("AnimationScene" == strCurScene)
	{
		GET_SINGLE(CToolValue)->m_pAnimationDialog->ChangePickObject(m_pGameObject);
	}	
}

CToolObject::CToolObject()
{
}

CToolObject::CToolObject(const CToolObject& _Object) :
	CScript(_Object)
{
}

CToolObject::~CToolObject()
{
}
