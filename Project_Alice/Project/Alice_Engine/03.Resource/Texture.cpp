#include "Texture.h"
#include "../01.Core/PathMgr.h"
#include "../Device.h"

WOOJUN_USING

CTexture * CTexture::CreateTexture(const string & _strKey, UINT _iWidth, UINT _iHeight, UINT _iArrSize,
	DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage, D3D11_BIND_FLAG _eBindFlag, int _iCpuFlag)
{
	CTexture*	pTexture = new CTexture();

	if (!pTexture->CreateResource(_strKey, _iWidth, _iHeight, _iArrSize, _eFormat, _eUsage, _eBindFlag, _iCpuFlag))	
	{
		SAFE_RELEASE(pTexture);
		return NULL;
	}

	return pTexture;
}

ID3D11Texture2D * CTexture::GetTexture() const
{
	return m_pTexture;
}

string CTexture::GetKey() const
{
	return m_strKey;
}

string CTexture::GetPathKey() const
{
	return m_strPathKey;
}

wstring CTexture::GetFullPath() const
{
	return m_strFullPath;
}

wstring CTexture::GetFileName() const
{
	return m_strFileName;
}

void CTexture::SetPathKey(const string & _strPathKey)
{
	m_strPathKey = _strPathKey;
}

bool CTexture::LoadTexture(const string & _strKey, TCHAR * _pFileName, const string & _strPathKey /*= TEXTUREPATH*/)
{
	// 유니코드 문자열을 멀티바이트 문자열로 만든다
	char	strPath[MAX_PATH] = {};

	WideCharToMultiByte(CP_ACP, 0, _pFileName, -1, strPath, lstrlen(_pFileName), NULL, NULL);

	m_strKey = _strKey;
	m_strPathKey = _strPathKey;
	m_strFileName = _pFileName;

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

	m_strKey = _strKey;
	m_strPathKey = _strPathKey;

	WCHAR	strFileName[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, _pFileName, -1, strFileName, strlen(_pFileName));

	m_strFileName = strFileName;

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

	m_strFullPath = strFullPath;
	m_strKey = _strKey;

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

	m_strPathKey = _strPathKey;
	m_strKey = _strKey;

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

	m_strPathKey = _strPathKey;
	m_strKey = _strKey;

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

bool CTexture::CreateResource(const string & _strKey, UINT _iWidth, UINT _iHeight, UINT _iArrSize, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage, D3D11_BIND_FLAG _eBindFlag, int _iCpuFlag)
{
	::DirectX::ScratchImage*	pImage = new ::DirectX::ScratchImage();

	if (FAILED(pImage->Initialize2D(_eFormat, _iWidth, _iHeight, _iArrSize, 1)))
	{
		SAFE_DELETE(pImage);
		return false;
	}

	m_vecScratchImage.push_back(pImage);
	
	if (FAILED(CreateShaderResourceViewEx(DEVICE, pImage->GetImages(), pImage->GetImageCount(),
		pImage->GetMetadata(), _eUsage, _eBindFlag, _iCpuFlag, 0, false, &m_pShaderResourceView)))
	{
		return false;
	}

	m_pShaderResourceView->GetResource((ID3D11Resource**)&m_pTexture);

	return true;
}

void CTexture::UpdateData(void * _pData, int _iSize)
{
	ID3D11Texture2D*	pTexture = NULL;
	m_pShaderResourceView->GetResource((ID3D11Resource**)&pTexture);

	D3D11_MAPPED_SUBRESOURCE	tMap;
	CONTEXT->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	memcpy(tMap.pData, _pData, _iSize);
	CONTEXT->Unmap(pTexture, 0);

	SAFE_RELEASE(pTexture);
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

void CTexture::SaveTextureFile(const WCHAR * _pFileName, const string & _strPathKey)
{
	const wchar_t*	pPath = GET_SINGLE(CPathMgr)->FindPath(_strPathKey);

	wstring	strFullPath;
	if (pPath)
		strFullPath = pPath;
	strFullPath += _pFileName;

	char	strFileName[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, _pFileName, -1, strFileName, lstrlen(_pFileName),
		0, 0);

	char	strExt[_MAX_EXT] = {};

	_splitpath_s(strFileName, NULL, 0, NULL, 0, NULL, 0, strExt, _MAX_EXT);

	_strupr_s(strExt);

	if (strcmp(strExt, ".DDS") == 0)
	{
		ScratchImage	img;
		CaptureTexture(DEVICE, CONTEXT, m_pTexture, img);
		SaveToDDSFile(img.GetImages(), img.GetImageCount(),
			img.GetMetadata(), DDS_FLAGS_NONE, strFullPath.c_str());
	}

	else if (strcmp(strExt, ".TGA") == 0)
	{
		ScratchImage	img;
		CaptureTexture(DEVICE, CONTEXT, m_pTexture, img);
		SaveToTGAFile(img.GetImages()[0], strFullPath.c_str());
	}

	else
	{
		ScratchImage	img;
		CaptureTexture(DEVICE, CONTEXT, m_pTexture, img);
		SaveToWICFile(img.GetImages(), img.GetImageCount(),
			WIC_FLAGS_NONE, GetWICCodec(WIC_CODEC_PNG), strFullPath.c_str());
	}
}

CTexture::CTexture() :
	m_pShaderResourceView(NULL),
	m_pTexture(NULL)
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pShaderResourceView);
	Safe_Delete_VecList(m_vecScratchImage);
}
