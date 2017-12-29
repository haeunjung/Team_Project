#include "Texture.h"
#include "../01.Core/PathMgr.h"
#include "../Device.h"

WOOJUN_USING

bool CTexture::LoadTexture(const string & _strKey, TCHAR * _pFileName, const string & _strPathKey /*= TEXTUREPATH*/)
{
	// 유니코드 문자열을 멀티바이트 문자열로 만든다
	char	strPath[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, _pFileName, -1, strPath, lstrlen(_pFileName), NULL, NULL);

	return LoadTexture(_strKey, strPath, _strPathKey);
}

bool CTexture::LoadTexture(const string & _strKey, CHAR * _pFileName, const string & _strPathKey /*= TEXTUREPATH*/)
{
	const char* pPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(_strPathKey);

	string strPath;
	if (NULL != pPath)
	{
		strPath = pPath;
	}

	strPath += _pFileName;

	return LoadTextureFromFullPath(_strKey, strPath.c_str());
}

bool CTexture::LoadTextureFromFullPath(const string & _strKey, const CHAR * _pFullPath)
{
	m_strKey = _strKey;

	char strExt[_MAX_EXT] = {};

	// 확장자명 구하기
	_splitpath_s(_pFullPath, NULL, 0, NULL, 0, NULL, 0, strExt, _MAX_EXT);

	// 대문자로 변환
	_strupr_s(strExt);

	::DirectX::ScratchImage*	pImage = new ::DirectX::ScratchImage();

	// 전체 경로를 유니코드로 변환한다.
	WCHAR	strFullPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, _pFullPath, -1, strFullPath, strlen(_pFullPath));

	// 확장자 DDS파일일 경우
	if (0 == strcmp(strExt, ".DDS"))
	{
		if (FAILED(LoadFromDDSFile(strFullPath, DDS_FLAGS_NONE, NULL, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}
	// 확장자 TGA파일일 경우
	else if (0 == strcmp(strExt, ".TGA"))
	{
		if (FAILED(LoadFromTGAFile(strFullPath, NULL, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}
	// 나머지 확장자
	else
	{
		if (FAILED(LoadFromWICFile(strFullPath, WIC_FLAGS_NONE, NULL, *pImage)))
		{
			SAFE_DELETE(pImage);
			return false;
		}
	}

	m_vecScratchImage.push_back(pImage);

	if (FAILED(CreateShaderResourceView(DEVICE, pImage->GetImages(), pImage->GetImageCount(), pImage->GetMetadata(), &m_pShaderResourceView)))
	{
		return false;
	}

	return true;
}

bool CTexture::LoadTexture(const string & _strKey, const vector<wstring>& _vecFileName, const string & _strPathKey)
{
	vector<string>	vecPath;

	size_t size = _vecFileName.size();
	for (size_t i = 0; i < size; ++i)
	{
		char	strPath[MAX_PATH] = {};

		WideCharToMultiByte(CP_ACP, 0, _vecFileName[i].c_str(), -1, strPath, _vecFileName[i].length(), NULL, NULL);

		vecPath.push_back(strPath);
	}

	return LoadTextureFromMultibyte(_strKey, vecPath, _strPathKey);
}

bool CTexture::LoadTextureFromMultibyte(const string & _strKey, const vector<string>& _vecFileName, const string & _strPathKey)
{
	const char* pPath = GET_SINGLE(CPathMgr)->FindPathToMultiByte(_strPathKey);

	string strPath;
	if (NULL != pPath)
	{
		strPath = pPath;
	}

	vector<string>	vecFullPath;

	size_t size = _vecFileName.size();
	for (size_t i = 0; i < size; ++i)
	{
		string	strFullPath = strPath;
		strFullPath += _vecFileName[i];

		vecFullPath.push_back(strFullPath);
	}

	return LoadTextureFromFullPath(_strKey, vecFullPath);
}

bool CTexture::LoadTextureFromFullPath(const string & _strKey, const vector<string>& _vecFullPath)
{
	m_strKey = _strKey;

	char strExt[_MAX_EXT] = {};

	size_t size = _vecFullPath.size();
	for (size_t i = 0; i < size; ++i)
	{
		// 확장자명 구하기
		_splitpath_s(_vecFullPath[i].c_str(), NULL, 0, NULL, 0, NULL, 0, strExt, _MAX_EXT);

		// 대문자로 변환
		_strupr_s(strExt);

		::DirectX::ScratchImage*	pImage = new ::DirectX::ScratchImage();

		// 전체 경로를 유니코드로 변환한다.
		WCHAR	strFullPath[MAX_PATH] = {};
		MultiByteToWideChar(CP_ACP, 0, _vecFullPath[i].c_str(), -1, strFullPath, _vecFullPath[i].length());

		// 확장자 DDS파일일 경우
		if (0 == strcmp(strExt, ".DDS"))
		{
			if (FAILED(LoadFromDDSFile(strFullPath, DDS_FLAGS_NONE, NULL, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}
		// 확장자 TGA파일일 경우
		else if (0 == strcmp(strExt, ".TGA"))
		{
			if (FAILED(LoadFromTGAFile(strFullPath, NULL, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}
		// 나머지 확장자
		else
		{
			if (FAILED(LoadFromWICFile(strFullPath, WIC_FLAGS_NONE, NULL, *pImage)))
			{
				SAFE_DELETE(pImage);
				return false;
			}
		}

		m_vecScratchImage.push_back(pImage);
	}

	vector<ID3D11Texture2D*>	vecTextureArray;

	size_t size2 = m_vecScratchImage.size();
	for (size_t i = 0; i < size2; ++i)
	{
		ID3D11Texture2D*	pTexture = NULL;
		if (FAILED(CreateTextureEx(DEVICE, m_vecScratchImage[i]->GetImages(), m_vecScratchImage[i]->GetImageCount(), m_vecScratchImage[i]->GetMetadata(),
			D3D11_USAGE_STAGING, 0, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ, 0, false, (ID3D11Resource**)&pTexture)))
		{
			Safe_Release_VecList(vecTextureArray);
			return false;
		}

		vecTextureArray.push_back(pTexture);
	}

	D3D11_TEXTURE2D_DESC	tTex2DDesc = {};
	vecTextureArray[0]->GetDesc(&tTex2DDesc);

	D3D11_TEXTURE2D_DESC	tTexArrayDesc = {};
	tTexArrayDesc.Width = tTex2DDesc.Width;
	tTexArrayDesc.Height = tTex2DDesc.Height;
	tTexArrayDesc.MipLevels = tTex2DDesc.MipLevels;
	tTexArrayDesc.ArraySize = vecTextureArray.size();
	tTexArrayDesc.Format = tTex2DDesc.Format;
	tTexArrayDesc.SampleDesc.Count = 1;
	tTexArrayDesc.SampleDesc.Quality = 0;
	tTexArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	tTexArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D*	pTexArray = NULL;
	if (FAILED(DEVICE->CreateTexture2D(&tTexArrayDesc, NULL, &pTexArray)))
	{
		Safe_Release_VecList(vecTextureArray);
		return false;
	}

	size_t size3 = vecTextureArray.size();
	for (size_t i = 0; i < size3; ++i)
	{
		for (UINT iMipLevel = 0; iMipLevel < tTex2DDesc.MipLevels; ++iMipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE	tMap = {};

			CONTEXT->Map(vecTextureArray[i], iMipLevel, D3D11_MAP_READ, 0, &tMap);
			CONTEXT->UpdateSubresource(pTexArray, D3D11CalcSubresource(iMipLevel, i, tTex2DDesc.MipLevels),
				NULL, tMap.pData, tMap.RowPitch, tMap.DepthPitch);
			CONTEXT->Unmap(vecTextureArray[i], iMipLevel);
		}
	}

	// Shader ResourceView Desc
	D3D11_SHADER_RESOURCE_VIEW_DESC	tViewDesc = {};
	tViewDesc.Format = tTexArrayDesc.Format;
	tViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	tViewDesc.Texture2DArray.MipLevels = tTexArrayDesc.MipLevels;
	tViewDesc.Texture2DArray.ArraySize = vecTextureArray.size();

	if (FAILED(DEVICE->CreateShaderResourceView(pTexArray, &tViewDesc, &m_pShaderResourceView)))
	{
		SAFE_RELEASE(pTexArray);
		Safe_Release_VecList(vecTextureArray);
		return false;
	}

	SAFE_RELEASE(pTexArray);
	Safe_Release_VecList(vecTextureArray);

	return true;
}

void CTexture::SetTexture(int _iRegister, int _iShaderType)
{
	if (_iShaderType & CUT_VERTEX)
	{
		CONTEXT->VSSetShaderResources(_iRegister, 1, &m_pShaderResourceView);
	}

	if (_iShaderType & CUT_PIXEL)
	{
		CONTEXT->PSSetShaderResources(_iRegister, 1, &m_pShaderResourceView);
	}
}

CTexture::CTexture() :
	m_pShaderResourceView(NULL)
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_pShaderResourceView);
	Safe_Delete_VecList(m_vecScratchImage);
}
