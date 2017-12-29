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
public:
	inline void* GetVSCode() const {
		return m_pVSBlob->GetBufferPointer();
	}
	inline void* GetPSCode() const {
		return m_pPSBlob->GetBufferPointer();
	}
	int GetVSCodeSize() const {
		return m_pVSBlob->GetBufferSize();
	}
	int GetPSCodeSize() const {
		return m_pPSBlob->GetBufferSize();
	}
public:
	bool LoadShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint[ST_END], const string& _strPathKey);
	bool LoadVertexShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint, const string& _strPathKey);
	bool LoadPixelShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint, const string& _strPathKey);
	bool LoadGeometryShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint, const string& _strPathKey);
	void SetShader();
private:
	CShader();
	virtual ~CShader();
};

WOOJUN_END