#include "Scene.h"
#include "Layer.h"
#include "SceneScript.h"
#include "../Engine_Core.h"
#include "06.GameObject/GameObject.h"
#include "07.Component/Camera.h"
#include "07.Component/Transform.h"
#include "07.Component/DirLight.h"
#include "07.Component/PointLight.h"
#include "07.Component/SpotLight.h"
#include "07.Component/Renderer.h"
#include "07.Component/Material.h"
#include "07.Component/Frustum.h"

WOOJUN_USING

bool CScene::LayerZSort(CLayer * _p1, CLayer * _p2)
{
	return _p1->GetZOrder() < _p2->GetZOrder();
}

CGameObject * CScene::GetMainCameraObject() const
{
	m_pCameraObject->AddRef();
	return m_pCameraObject;
}

CCamera * CScene::GetMainCamera() const
{
	m_pCamera->AddRef();
	return m_pCamera;
}

CTransform * CScene::GetMainCameraTransform() const
{
	return m_pCameraObject->GetTransform();;
}

CFrustum * CScene::GetMainCameraFrustum() const
{
	m_pFrustum->AddRef();
	return m_pFrustum;
}

CGameObject * CScene::FindCameraObject(const string & _strKey) const
{
	unordered_map<string, CGameObject*>::const_iterator	iter = m_mapCamera.find(_strKey);

	if (m_mapCamera.end() == iter)
	{
		return NULL;
	}

	iter->second->AddRef();

	return iter->second;
}

CCamera * CScene::FindCamera(const string & _strKey) const
{
	unordered_map<string, CGameObject*>::const_iterator	iter = m_mapCamera.find(_strKey);

	if (m_mapCamera.end() == iter)
	{
		return NULL;
	}

	CCamera*	pCamera = iter->second->FindComponentFromTypeID<CCamera>();

	return pCamera;
}

CTransform * CScene::FindCameraTransform(const string & _strKey) const
{
	unordered_map<string, CGameObject*>::const_iterator	iter = m_mapCamera.find(_strKey);

	if (m_mapCamera.end() == iter)
	{
		return NULL;
	}
	
	return iter->second->GetTransform();;
}

CFrustum * CScene::FindFrustum(const string & _strKey) const
{
	unordered_map<string, CGameObject*>::const_iterator	iter = m_mapCamera.find(_strKey);

	if (m_mapCamera.end() == iter)
	{
		return NULL;
	}

	CFrustum*	pFrustum = iter->second->FindComponentFromTypeID<CFrustum>();

	return pFrustum;
}

const list<CGameObject*>* CScene::GetLightList() const
{
	return &m_LightList;
}

const string & CScene::GetTag() const
{	
	return m_strTag;
}

CLayer * CScene::CreateLayer(const string & _strTag, int _iZOrder /*= 0*/)
{
	CLayer* pLayer = new CLayer();

	if (false == pLayer->Init())
	{
		SAFE_RELEASE(pLayer);
		return NULL;
	}

	pLayer->SetTag(_strTag);
	pLayer->SetZOrder(_iZOrder);
	pLayer->SetScene(this);

	pLayer->AddRef();

	m_vecLayer.push_back(pLayer);

	if (m_vecLayer.size() >= 2)
	{
		sort(m_vecLayer.begin(), m_vecLayer.end(), CScene::LayerZSort);
	}

	return pLayer;
}

CLayer * CScene::FindLayer(const string & _strTag)
{
	size_t vecLayerSize = m_vecLayer.size();
	for (size_t i = 0; i < vecLayerSize; ++i)
	{
		if (_strTag == m_vecLayer[i]->GetTag())
		{
			m_vecLayer[i]->AddRef();
			return m_vecLayer[i];
		}
	}

	return NULL;
}

CGameObject * CScene::CreateCamera(const string & _strKey, const DxVector3 & _vPos)
{
	CGameObject* pCameraObject = FindCameraObject(_strKey);

	if (NULL != pCameraObject)
	{
		// 이미 있으면 있는거 리턴하자
		return pCameraObject;
	}

	pCameraObject = CGameObject::Create(_strKey);

	CTransform* pTransform = pCameraObject->GetTransform();
	pTransform->SetWorldPos(_vPos);
	SAFE_RELEASE(pTransform);

	CCamera* pCamera = pCameraObject->AddComponent<CCamera>("Camera");	
	SAFE_RELEASE(pCamera);

	CFrustum* pFrustum = pCameraObject->AddComponent<CFrustum>("Frustum");
	SAFE_RELEASE(pFrustum);

	pCameraObject->AddRef();
	pCameraObject->SetScene(this);

	m_mapCamera.insert(make_pair(_strKey, pCameraObject));

	return pCameraObject;
}

bool CScene::CreateProjection(const string & _strKey, float _fAngle, float _fWidth, float _fHeight, float _fNear /*= 0.3f*/, float _fFar /*= 1000.0f*/)
{
	// _strKey로 카메라를 찾고 원근투영행렬 세팅
	CCamera* pCamera = FindCamera(_strKey);

	if (NULL == pCamera)
	{
		assert(false);
		return false;
	}

	pCamera->CreateProjection(_fAngle, _fWidth, _fHeight, _fNear, _fFar);

	SAFE_RELEASE(pCamera);

	return true;
}

bool CScene::CreateOrthoProjection(const string & _strKey, float _fWidth, float _fHeight, float _fNear /*= 0.0f*/, float _fFar /*= 1000.0f*/)
{
	// _strKey로 카메라를 찾고 직교투영행렬 세팅
	CCamera* pCamera = FindCamera(_strKey);

	if (NULL == pCamera)
	{
		assert(false);
		return false;
	}

	pCamera->CreateOrthoProjection(_fWidth, _fHeight, _fNear, _fFar);

	SAFE_RELEASE(pCamera);

	return true;
}

bool CScene::CreateProjection(float _fAngle, float _fWidth, float _fHeight, float _fNear /*= 0.3f*/, float _fFar /*= 1000.0f*/)
{
	m_pCamera->CreateProjection(_fAngle, _fWidth, _fHeight, _fNear, _fFar);

	return true;
}

bool CScene::CreateOrthoProjection(float _fWidth, float _fHeight, float _fNear /*= 0.0f*/, float _fFar /*= 1000.0f*/)
{
	m_pCamera->CreateOrthoProjection(_fWidth, _fHeight, _fNear, _fFar);

	return true;
}

bool CScene::ChangeCamera(const string & _strKey)
{
	CGameObject* pCameraObject = FindCameraObject(_strKey);

	if (NULL == pCameraObject)
	{
		return false;
	}

	SAFE_RELEASE(m_pCameraObject);
	SAFE_RELEASE(m_pCamera);
	
	m_pCameraObject = pCameraObject;
	m_pCamera = (CCamera*)m_pCameraObject->FindComponentFromType(CT_CAMERA);

	return true;
}

CGameObject * CScene::CreateLight(const string & _strKey, LIGHT_TYPE _eType)
{
	CGameObject*	pLightObject = CGameObject::Create(_strKey);

	CLight*	pLight = NULL;

	switch (_eType)
	{
	case LT_DIR:
		pLight = pLightObject->AddComponent<CDirLight>(_strKey);
		break;
	case LT_POINT:
		pLight = pLightObject->AddComponent<CPointLight>(_strKey);
		break;
	case LT_SPOT:
		pLight = pLightObject->AddComponent<CSpotLight>(_strKey);
		break;
	case LT_END:
		break;
	default:
		break;
	}

	SAFE_RELEASE(pLight);

	pLightObject->AddRef();

	m_LightList.push_back(pLightObject);

	return pLightObject;
}

CGameObject * CScene::FindLight(const string & _strKey)
{
	return nullptr;
}

bool CScene::Init()
{
	CLayer* pLayer = CreateLayer(DEFAULTLAYER, 1);	
	pLayer->SetSortFlag(SF_DESCENDING);
	SAFE_RELEASE(pLayer);

	CLayer* pMapLayer = CreateLayer(MAPLAYER);
	SAFE_RELEASE(pMapLayer);

	CLayer* pUILayer = CreateLayer("UILayer", INT_MAX);
	SAFE_RELEASE(pUILayer);

	CLayer* pEnvironmentLayer = CreateLayer("EnvironmentLayer", -1);
	// 기본 환경맵 생성
	CGameObject*	pEnvironmentObject = CGameObject::Create("Environment");
	
	CTransform*		pTransform = pEnvironmentObject->GetTransform();
	pTransform->SetWorldScale(1000.0f, 1000.0f, 1000.0f);

	SAFE_RELEASE(pTransform);

	CRenderer*	pRenderer = pEnvironmentObject->AddComponent<CRenderer>("Environment");
	pRenderer->SetMesh("PosSphere");
	pRenderer->SetShader("SkyShader");
	pRenderer->SetInputLayout("PosInputLayout");
	pRenderer->SetRenderState(CULLING_CW);	
	pRenderer->SetRenderState(DEPTH_LESS_EQUAL);
	
	CMaterial*	pMaterial = pRenderer->GetMaterial();
	pMaterial->SetDiffuseTexture("Linear", "Sky", L"Sky.dds");
		
	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);

	pEnvironmentLayer->AddObject(pEnvironmentObject);
	SAFE_RELEASE(pEnvironmentLayer);
	SAFE_RELEASE(pEnvironmentObject);
	
	// Scene 생성시 Main Camera 생성
	m_pCameraObject = CreateCamera(MAINCAMERA, DxVector3(0.0f, 0.0f, -5.0f));
	m_pCamera = (CCamera*)m_pCameraObject->FindComponentFromTypeID<CCamera>();
	m_pFrustum = (CFrustum*)m_pCameraObject->FindComponentFromTypeID<CFrustum>();

	// UI Camera
	CGameObject*	pUICameraObject = CreateCamera("UICamera", DxVector3(0.0f, 0.0f, -5.0f));
	CreateOrthoProjection("UICamera", _RESOLUTION.m_iWidth, _RESOLUTION.m_iHeight);
	SAFE_RELEASE(pUICameraObject);

	// Create Default Global Light 
	CGameObject*	pLightObject = CreateLight("GlobalLight", LT_DIR);

	pTransform = pLightObject->GetTransform();
	pTransform->RotateX(PI / 4.0f);
	pTransform->RotateY(PI / 2.0f);
	SAFE_RELEASE(pTransform);

	/*CGameObject*	pLightObject = CreateLight("PointLight", LT_POINT);
	
	CLight* pLight = (CLight*)pLightObject->FindComponentFromType(CT_LIGHT);

	LIGHTINFO	tLightInfo = {};
	tLightInfo.eType = LT_POINT;
	tLightInfo.vDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	tLightInfo.vAmbient = { 0.3f, 0.3f, 0.3f, 1.0f };
	tLightInfo.vSpecular = { 1.0f, 1.0f, 1.0f, 3.2f };
	tLightInfo.vAttenuation = DxVector3(0.0f, 1.0f, 0.0f);
	pLight->SetLightInfo(tLightInfo);

	SAFE_RELEASE(pLight);*/
		
	//pUILayer->AddObject(pLightObject);
	
	SAFE_RELEASE(pLightObject);

	return true;
}

void CScene::Input(float _fTime)
{
	for (size_t i = 0; i < m_vecSceneScript.size(); ++i)
	{
		m_vecSceneScript[i]->Input(_fTime);
	}

	list<CGameObject*>::iterator	iterObj;
	list<CGameObject*>::iterator	iterObjEnd = m_LightList.end();
	for (iterObj = m_LightList.begin(); iterObj != iterObjEnd;)
	{
		if (false == (*iterObj)->GetIsEnable())
		{
			++iterObj;
			continue;
		}

		(*iterObj)->Input(_fTime);

		if (false == (*iterObj)->GetIsAlive())
		{
			SAFE_RELEASE((*iterObj));
			iterObj = m_LightList.erase(iterObj);
		}
		else
		{
			++iterObj;
		}
	}

	m_pCameraObject->Input(_fTime);
	
	vector<CLayer*>::iterator	iterLayer;
	vector<CLayer*>::iterator	iterLayerEnd = m_vecLayer.end();
	for (iterLayer = m_vecLayer.begin(); iterLayer != iterLayerEnd;)
	{
		if (false == (*iterLayer)->GetIsEnable())
		{
			++iterLayer;
			continue;
		}

		(*iterLayer)->Input(_fTime);

		if (false == (*iterLayer)->GetIsAlive())
		{
			SAFE_RELEASE((*iterLayer));
			iterLayer = m_vecLayer.erase(iterLayer);
		}
		else
		{
			++iterLayer;
		}
	}	
}

void CScene::Update(float _fTime)
{
	for (size_t i = 0; i < m_vecSceneScript.size(); ++i)
	{
		m_vecSceneScript[i]->Update(_fTime);
	}

	list<CGameObject*>::iterator	iterObj;
	list<CGameObject*>::iterator	iterObjEnd = m_LightList.end();
	for (iterObj = m_LightList.begin(); iterObj != iterObjEnd;)
	{
		if (false == (*iterObj)->GetIsEnable())
		{
			++iterObj;
			continue;
		}

		(*iterObj)->Update(_fTime);

		if (false == (*iterObj)->GetIsAlive())
		{
			SAFE_RELEASE((*iterObj));
			iterObj = m_LightList.erase(iterObj);
		}
		else
		{
			++iterObj;
		}
	}

	vector<CLayer*>::iterator	iterLayer;
	vector<CLayer*>::iterator	iterLayerEnd = m_vecLayer.end();
	for (iterLayer = m_vecLayer.begin(); iterLayer != iterLayerEnd;)
	{
		if (false == (*iterLayer)->GetIsEnable())
		{
			++iterLayer;
			continue;
		}

		(*iterLayer)->Update(_fTime);

		if (false == (*iterLayer)->GetIsAlive())
		{
			SAFE_RELEASE((*iterLayer));
			iterLayer = m_vecLayer.erase(iterLayer);
		}
		else
		{
			++iterLayer;
		}
	}

	m_pCameraObject->Update(_fTime);
}

void CScene::LateUpdate(float _fTime)
{
	for (size_t i = 0; i < m_vecSceneScript.size(); ++i)
	{
		m_vecSceneScript[i]->LateUpdate(_fTime);
	}

	list<CGameObject*>::iterator	iterObj;
	list<CGameObject*>::iterator	iterObjEnd = m_LightList.end();
	for (iterObj = m_LightList.begin(); iterObj != iterObjEnd;)
	{
		if (false == (*iterObj)->GetIsEnable())
		{
			++iterObj;
			continue;
		}

		(*iterObj)->LateUpdate(_fTime);

		if (false == (*iterObj)->GetIsAlive())
		{
			SAFE_RELEASE((*iterObj));
			iterObj = m_LightList.erase(iterObj);
		}
		else
		{
			++iterObj;
		}
	}

	vector<CLayer*>::iterator	iterLayer;
	vector<CLayer*>::iterator	iterLayerEnd = m_vecLayer.end();
	for (iterLayer = m_vecLayer.begin(); iterLayer != iterLayerEnd;)
	{
		if (false == (*iterLayer)->GetIsEnable())
		{
			++iterLayer;
			continue;
		}

		(*iterLayer)->LateUpdate(_fTime);

		if (false == (*iterLayer)->GetIsAlive())
		{
			SAFE_RELEASE((*iterLayer));
			iterLayer = m_vecLayer.erase(iterLayer);
		}
		else
		{
			++iterLayer;
		}
	}

	m_pCameraObject->LateUpdate(_fTime);

	bool	bSort = false;
	for (size_t i = 0; i < m_vecLayer.size(); ++i)
	{
		if (true == m_vecLayer[i]->GetZOrderEnable())
		{
			bSort = true;
			m_vecLayer[i]->DisableZOrder();
		}
	}
		
	sort(m_vecLayer.begin(), m_vecLayer.end(), CScene::LayerZSort);
}

void CScene::Collision(float _fTime)
{
	for (size_t i = 0; i < m_vecSceneScript.size(); ++i)
	{
		m_vecSceneScript[i]->Collision(_fTime);
	}

	vector<CLayer*>::iterator	iterLayer;
	vector<CLayer*>::iterator	iterLayerEnd = m_vecLayer.end();
	for (iterLayer = m_vecLayer.begin(); iterLayer != iterLayerEnd;)
	{
		if (false == (*iterLayer)->GetIsEnable())
		{
			++iterLayer;
			continue;
		}

		(*iterLayer)->Collision(_fTime);

		if (false == (*iterLayer)->GetIsAlive())
		{
			SAFE_RELEASE((*iterLayer));
			iterLayer = m_vecLayer.erase(iterLayer);
		}
		else
		{
			++iterLayer;
		}
	}

	m_pCameraObject->Collision(_fTime);
}

void CScene::Render(float _fTime)
{
	for (size_t i = 0; i < m_vecSceneScript.size(); ++i)
	{
		m_vecSceneScript[i]->Render(_fTime);
	}

	list<CGameObject*>::iterator	iterObj;
	list<CGameObject*>::iterator	iterObjEnd = m_LightList.end();
	for (iterObj = m_LightList.begin(); iterObj != iterObjEnd;)
	{
		if (false == (*iterObj)->GetIsEnable())
		{
			++iterObj;
			continue;
		}

		(*iterObj)->Render(_fTime);

		if (false == (*iterObj)->GetIsAlive())
		{
			SAFE_RELEASE((*iterObj));
			iterObj = m_LightList.erase(iterObj);
		}
		else
		{
			++iterObj;
		}
	}

	vector<CLayer*>::iterator	iterLayer;
	vector<CLayer*>::iterator	iterLayerEnd = m_vecLayer.end();
	for (iterLayer = m_vecLayer.begin(); iterLayer != iterLayerEnd;)
	{
		if (false == (*iterLayer)->GetIsEnable())
		{
			++iterLayer;
			continue;
		}

		(*iterLayer)->Render(_fTime);

		if (false == (*iterLayer)->GetIsAlive())
		{
			SAFE_RELEASE((*iterLayer));
			iterLayer = m_vecLayer.erase(iterLayer);
		}
		else
		{
			++iterLayer;
		}
	}

	m_pCameraObject->Render(_fTime);
}

CScene::CScene() :
	m_pCameraObject(NULL),
	m_pCamera(NULL),
	m_pFrustum(NULL)
{
}

CScene::~CScene()
{
	for (size_t i = 0; i < m_vecSceneScript.size(); ++i)
	{
		SAFE_DELETE(m_vecSceneScript[i]);
	}
	m_vecSceneScript.clear();
	Safe_Release_VecList(m_vecLayer);	

	Safe_Release_Map(m_mapCamera);
	SAFE_RELEASE(m_pFrustum);
	SAFE_RELEASE(m_pCamera);
	SAFE_RELEASE(m_pCameraObject);
	Safe_Release_VecList(m_LightList);
}
