#pragma once
#include "Component.h"

WOOJUN_BEGIN

class CMesh;
class CShader;
class CRenderState;
class CMaterial;
class CTexture;
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
	unordered_map<string, pRENDERERCBUFFER>	m_mapCBuffer;
	CTexture*	m_pBoneTexture;
	bool		m_bBlend;
private:	
	void UpdateTransform();
	void CheckAnimation();
public:
	bool BlendEnable() const;
	void SetBoneTexture(CTexture* _pBoneTexture);
// Mesh
public:
	CMesh* GetMesh() const;
	DxVector3 GetMeshMin() const;
	DxVector3 GetMeshMax() const;
	DxVector3 GetMeshSize() const;
	SPHEREINFO GetSphereInfo() const;
	DxVector3 GetCenter() const;
	float GetRadius() const;
public:
	void SetMesh(const string& _strKey);
	void SetMesh(const string& _strKey, const TCHAR* _pFileName, const string& _strPathKey = MESHPATH);
	void SetMesh(const string& _strKey, const CHAR* _pFileName, const string& _strPathKey = MESHPATH);
	void SetMeshFromFullPath(const string& _strKey, const TCHAR* _pFullPath);
	void SetMesh(CMesh* _pMesh);
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

// Renderer CBuffer
public:
	void AddConstBuffer(const string& _strKey, int _iRegister, int _iSize, int _iShaderType);
	bool UpdateCBuffer(const string& _strKey, void* _pData);
	pRENDERERCBUFFER FindConstBuffer(const string& _strKey);
public:
	bool Init() override;
	void Input(float _fTime) override;
	void Update(float _fTime) override;
	void LateUpdate(float _fTime) override;
	void Collision(float _fTime) override;
	void Render(float _fTime) override;
	CRenderer* Clone() override;
private:
	CRenderer();
	CRenderer(const CRenderer& _Renderer);
	virtual ~CRenderer();
};

WOOJUN_END