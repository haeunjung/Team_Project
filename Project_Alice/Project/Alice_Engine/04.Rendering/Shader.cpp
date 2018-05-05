#include "Shader.h"
#include "..\Device.h"
#include "..\01.Core\PathMgr.h"

WOOJUN_USING

void CShader::SetStreamDecl(D3D11_SO_DECLARATION_ENTRY * pStreamDecl, UINT iCount)
{
	m_pStreamDecl = pStreamDecl;
	m_iDeclCount = iCount;
}

bool CShader::LoadShader(const string& _strKey, WCHAR* _pFileName, char* _pEntryPoint[ST_END], bool _bStreamOut, const string& _strPathKey)
{
	if (false == LoadVertexShader(_strKey, _pFileName, _pEntryPoint[ST_VERTEX], _strPathKey))
	{
		return false;
	}

	if (false == LoadPixelShader(_strKey, _pFileName, _pEntryPoint[ST_PIXEL], _strPathKey))
	{
		return false;
	}

	if (_pEntryPoint[ST_GEOMETRY])
	{
		if (false == LoadGeometryShader(_strKey, _pFileName, _pEntryPoint[ST_GEOMETRY], _bStreamOut, _strPathKey))
		{
			return false;
		}
	}

	return true;
}

bool CShader::LoadVertexShader(const string & _strKey, WCHAR * _pFileName, char * _pEntryPoint, const string & _strPathKey)
{
	if (!_pEntryPoint)
		return true;

	m_strKey = _strKey;
	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag |= D3D10_SHADER_DEBUG;
#endif

	const WCHAR* pPath = GET_SINGLE(CPathMgr)->FindPath(_strPathKey);
	wstring strPath;

	if (NULL != pPath)
	{
		strPath = pPath;
	}
	strPath += _pFileName;

	ID3DBlob* pErrorBlob = NULL;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _pEntryPoint, "vs_5_0", iFlag, 0,
		&m_pVSBlob, &pErrorBlob)))
	{
		_cprintf("%s\n", (char*)pErrorBlob->GetBufferPointer());
		return false;
	}
	
	if (FAILED(DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), NULL, &m_pVS)))
	{
		return false;
	}

	return true;
}

bool CShader::LoadPixelShader(const string & _strKey, WCHAR * _pFileName, char * _pEntryPoint, const string & _strPathKey)
{
	if (!_pEntryPoint)
		return true;

	m_strKey = _strKey;
	unsigned int iFlag = 0;	

#ifdef _DEBUG
	iFlag |= D3D10_SHADER_DEBUG;
#endif

	const WCHAR* pPath = GET_SINGLE(CPathMgr)->FindPath(_strPathKey);
	wstring strPath;

	if (NULL != pPath)
	{
		strPath = pPath;
	}
	strPath += _pFileName;

	ID3DBlob* pErrorBlob = NULL;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _pEntryPoint, "ps_5_0", iFlag, 0,
		&m_pPSBlob, &pErrorBlob)))
	{
		return false;
	}

	if (FAILED(DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), NULL, &m_pPS)))
	{
		return false;
	}

	return true;
}

bool CShader::LoadGeometryShader(const string & _strKey, WCHAR * _pFileName, char * _pEntryPoint, bool _bStreamOut, const string & _strPathKey)
{
	if (!_pEntryPoint)
		return true;

	m_strKey = _strKey;
	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag |= D3D10_SHADER_DEBUG;
#endif
	// 전체경로를 만들어준다.
	const WCHAR* pPath = GET_SINGLE(CPathMgr)->FindPath(_strPathKey);
	wstring strPath;

	if (pPath)
	{
		strPath = pPath;
	}
	strPath += _pFileName;

	ID3DBlob* pErrorBlob = NULL;
	if (FAILED(D3DCompileFromFile(strPath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, _pEntryPoint, "gs_5_0", iFlag, 0,
		&m_pGSBlob, &pErrorBlob)))
	{
		return false;
	}

	if (!_bStreamOut)
	{
		if (FAILED(DEVICE->CreateGeometryShader(m_pGSBlob->GetBufferPointer(), m_pGSBlob->GetBufferSize(),
			NULL, &m_pGS)))
		{
			return false;
		}
	}
	else
	{
		if (FAILED(DEVICE->CreateGeometryShaderWithStreamOutput(m_pGSBlob->GetBufferPointer(),
			m_pGSBlob->GetBufferSize(), m_pStreamDecl, m_iDeclCount, NULL, 0,
			D3D11_SO_NO_RASTERIZED_STREAM, NULL, &m_pGS)))
			return false;
	}

	return true;
}

void CShader::SetShader()
{
	CONTEXT->VSSetShader(m_pVS, NULL, 0);
	CONTEXT->PSSetShader(m_pPS, NULL, 0);	
	CONTEXT->GSSetShader(m_pGS, NULL, 0);
}

CShader::CShader() :
	m_pVSBlob(NULL),
	m_pVS(NULL),
	m_pPSBlob(NULL),
	m_pPS(NULL),
	m_pGSBlob(NULL),
	m_pGS(NULL)
{
}


CShader::~CShader()
{
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pGSBlob);
}
