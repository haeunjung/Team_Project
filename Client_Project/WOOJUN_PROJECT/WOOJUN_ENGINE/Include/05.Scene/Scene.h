#pragma once
#include "../Engine_Header.h"

WOOJUN_BEGIN

class CLayer;
class CSceneScript;
class CGameObject;
class CCamera;
class CTransform;
class DLL CScene 
{
private:
	friend class CSceneMgr;
private:
	string m_strTag;
	vector<CLayer*> m_vecLayer;	
	vector<CSceneScript*> m_vecSceneScript;	
	unordered_map<string, CGameObject*> m_mapCamera;
	CGameObject* m_pCameraObject;
	CCamera* m_pCamera;
	list<CGameObject*>	m_LightList;	
public:
	static bool LayerZSort(CLayer* _p1, CLayer* _p2);
public:
	CGameObject* GetMainCameraObject() const;
	CCamera* GetMainCamera() const;
	CTransform* GetMainCameraTransform() const;

	CGameObject* FindCameraObject(const string& _strKey) const;
	CCamera* FindCamera(const string& _strKey) const;
	CTransform* FindCameraTransform(const string& _strKey) const;

	const list<CGameObject*>* GetLightList() const;
public:
	inline void SetTag(const string& _strTag) {
		m_strTag = _strTag;
	}
public:
	CLayer* CreateLayer(const string& _strTag, int _iZOrder = 0);
	template <typename T>
	T* CreateScript()
	{
		T*		pScript = new T();
		pScript->m_pScene = this;

		if (false == pScript->Init())
		{
			SAFE_DELETE(pScript);
			return NULL;
		}

		m_vecSceneScript.push_back(pScript);

		return pScript;
	}
	CLayer* FindLayer(const string& _strTag);
public:
	CGameObject* CreateCamera(const string& _strKey, const DxVector3& _vPos = Vec3Zero);
	bool CreateProjection(const string& _strKey, float _fAngle, float _fWidth, float _fHeight, float _fNear = 0.3f, float _fFar = 1000.0f);
	bool CreateOrthoProjection(const string& _strKey, float _fWidth, float _fHeight, float _fNear = 0.0f, float _fFar = 1000.0f);
	bool CreateProjection(float _fAngle, float _fWidth, float _fHeight, float _fNear = 0.3f, float _fFar = 1000.0f);
	bool CreateOrthoProjection(float _fWidth, float _fHeight, float _fNear = 0.0f, float _fFar = 1000.0f);	
	bool ChangeCamera(const string& _strKey);
public:
	CGameObject* CreateLight(const string& _strKey, LIGHT_TYPE _eType);
	CGameObject* FindLight(const string& _strKey);
public:
	bool Init();
	void Input(float _fTime);
	void Update(float _fTime);
	void LateUpdate(float _fTime);
	void Collision(float _fTime);
	void Render(float _fTime);
private:
	CScene();
	~CScene();
};

WOOJUN_END