#pragma once
#include "Component.h"

WOOJUN_BEGIN

class CMesh;
class CShader;
class CRenderState;
class CMaterial;
class DLL CRenderer : public CComponent
{
private:
	friend class CGameObject;
private:
	CMesh*		m_pMesh;
	CShader*	m_pShader;
	ID3D11InputLayout*	m_pInputLayout;
	CRenderState*		m_pRenderState[RST_END];
	vector<vector<CMaterial*>>	m_vecMaterial;
private:
	void UpdateTransform();

// Mesh
public:
	CMesh* GetMesh() const;
public:
	void SetMesh(const string& _strKey);
	void SetMesh(const string& _strKey, const TCHAR* _pFileName, const string& _strPathKey = MESHPATH);
	void SetShader(const string& _strKey);
	void SetInputLayout(const string& _strKey);
	void SetRenderState(const string& _strKey);

// Material
public:
	CMaterial* GetMaterial(int _iContainer = 0, int _iSubSet = 0);
public:
	CMaterial* CreateMaterial(const string& _strSamplerKey, const string& _strTextureKey, 
		int _iSamplerRegister = 0, int _iTextureRegister = 0, 
		TCHAR* _pFileName = NULL, const string& _strPathKey = TEXTUREPATH,
		int _iContainer = 0);
	void AddMaterial(CMaterial* _pMaterial, int _iContainer = 0);
	void AddContainerMaterial();
public:
	virtual bool Init() override;
	virtual void Input(float _fTime) override;
	virtual void Update(float _fTime) override;
	virtual void LateUpdate(float _fTime) override;
	virtual void Collision(float _fTime) override;
	virtual void Render(float _fTime) override;
	virtual CRenderer* Clone() override;
private:
	CRenderer();
	CRenderer(const CRenderer& _Renderer);
	virtual ~CRenderer();
};

WOOJUN_END