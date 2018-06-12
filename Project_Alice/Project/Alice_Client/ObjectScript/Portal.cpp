#include "Portal.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Transform.h"
#include "07.Component/Renderer.h"

CPortal::CPortal()
{
	SetTag("Portal");
	SetTypeName("CPortal");
	SetTypeID<CPortal>();
}

CPortal::~CPortal()
{
}

bool CPortal::Init()
{
	m_pTransform->SetWorldPos(10.0f, 0.0f, 10.0f);
	m_pTransform->SetWorldScale(0.02f, 0.02f, 0.02f);
	//m_pTransform->SetWorldRot(_vRot);

	CRenderer* pRenderer = m_pGameObject->AddComponent<CRenderer>("PortalRenderer");
	pRenderer->SetMesh("Portal", "Portal.FBX");
	pRenderer->SetShader(STANDARD_BUMP_SHADER);
	pRenderer->SetInputLayout("BumpInputLayout");

	DxVector3 vMeshSize = pRenderer->GetMeshSize();
	SAFE_RELEASE(pRenderer);

	//CColliderSphere* pColSphere = pGameObject->AddComponent<CColliderSphere>(_strKey + "ColSphere");
	//pColSphere->SetSphereInfo(_vPos, 1.0f);
	//SAFE_RELEASE(pColSphere);

	/*CColliderAABB* pColAABB = pGameObject->AddComponent<CColliderAABB>(_strKey + "ColAABB");
	pColAABB->SetColCheck(CC_OBJ);
	pColAABB->SetScale(vMeshSize);
	SAFE_RELEASE(pColAABB);*/

	return true;
}

void CPortal::Update(float _fTime)
{
}


