#pragma once
#include "..\BaseObj.h"

WOOJUN_BEGIN

class DLL CShader : public CBaseObj
{
private:
	friend class CShaderMgr;
private:
	ID3DBlob*				m_pVSBlob;
	ID3D11VertexShader*		m_pVS;

	ID3DBlob*				m_pPSBlob;
	ID3D11PixelShader*		m_pPS;

	ID3DBlob*				m_pGSBlob;
	ID3D11GeometryShader*	m_pGS;

	string				m_strKey;

	D3D11_SO_DECLARATION_ENTRY*		m_pStreamDecl;
	UINT							m_iDeclCount;
public:
	inline void* GetVSCode() const {
		return m_pVSBlob->GetBufferPointer();
	}
	inline void* GetPSCode() const {
		return m_pPSBlob->GetBufferPointer();
	}
	inline int GetVSCodeSize() const {
		return m_pVSBlob->GetBufferSize();
	}
	inline int GetPSCodeSize() const {
		return m_pPSBlob->GetBufferSize();
	}
	inline string GetKey() const {
		return m_strKey;
	}
private:
	void SetStreamDecl(D3D11_SO_DECLARATION_ENTRY* pStreamDecl, UINT iCount);
public:
	bool LoadShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint[ST_END], bool _bStreamOut = false, const string& _strPathKey = SHADERPATH);
	bool LoadVertexShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint, const string& _strPathKey);
	bool LoadPixelShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint, const string& _strPathKey);
	bool LoadGeometryShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint, bool _bStreamOut, const string& _strPathKey);
	void SetShader();
private:
	CShader();
	virtual ~CShader();
};

WOOJUN_END