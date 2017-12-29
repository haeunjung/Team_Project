#pragma once
#include "../Engine_Header.h"

WOOJUN_BEGIN

class CShader;
class DLL CShaderMgr
{
private:
	unordered_map<string, CShader*> m_mapShader;
	unordered_map<string, ID3D11InputLayout*> m_mapInputLayout;
	unordered_map<string, pCONSTBUFFER> m_mapCBuffer;

	vector<D3D11_INPUT_ELEMENT_DESC> m_vecElement;
	
	unsigned int m_iOffsetSize;
public:
	bool Init();
	// Shader
	CShader* LoadShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint[ST_END], const string& _strPathKey = SHADERPATH);
	CShader* FindShader(const string& _strKey);
		void AddElement(char* _pSemanticName, int _iSemanticIndex, int _iSize, DXGI_FORMAT _eFromat, int _iInputSlot,
		D3D11_INPUT_CLASSIFICATION _eSlotClass, int _iInstanceStep);
	// InputLayout
	bool CreateInputLayout(const string& _strKey, const string& _strShaderKey);
	ID3D11InputLayout* FindInputLayout(const string& _strKey);
	// Const Buffer
	bool CreateConstBuffer(const string& _strKey, int _iSize, int _iRegister);
	pCONSTBUFFER FindConstBuffer(const string _strKey);
	void UpdateConstBuffer(const string& _strKey, void* _pData, int _iCut);

	DECLARE_SINGLE(CShaderMgr)
};

WOOJUN_END