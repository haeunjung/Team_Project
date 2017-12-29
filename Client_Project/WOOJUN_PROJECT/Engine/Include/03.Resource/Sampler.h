#pragma once
#include "../BaseObj.h"

WOOJUN_BEGIN

class DLL CSampler :
	public CBaseObj
{
private:
	friend class CResMgr;
private:
	ID3D11SamplerState*		m_pSampler;
public:
	bool CreateSampler(const string& _strKey,
		D3D11_FILTER _eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE _eAddrU = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE _eAddrV = D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_MODE _eAddrW = D3D11_TEXTURE_ADDRESS_WRAP);
	void SetSampler(int _iRegister, int _iShaderType);
private:
	CSampler();
	~CSampler();
};

WOOJUN_END