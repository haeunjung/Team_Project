#include "ShaderMgr.h"
#include "..\Device.h"
#include "Shader.h"

WOOJUN_USING

DEFINITION_SINGLE(CShaderMgr)

bool CShaderMgr::Init()
{
	// STANDARD COLORSHADER LOAD
	char* pEntryPoint[ST_END] = { "StandardColorVS", "StandardColorPS", NULL };
	CShader* pShader = LoadShader("StandardColorShader", L"Standard.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// COLLIDER SHADER LOAD
	pEntryPoint[ST_VERTEX] = "ColliderVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "ColliderPS";
	pShader = LoadShader("ColliderColorShader", L"Collider.fx", pEntryPoint);
	SAFE_RELEASE(pShader);
	
	pEntryPoint[ST_VERTEX] = "StandardTextureNormalVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "StandardTextureNormalPS";
	pShader = LoadShader(STANDARD_TEXNORMAL_SHADER, L"Standard.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// Bump Shader
	pEntryPoint[ST_VERTEX] = "StandardBumpVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "StandardBumpPS";
	pShader = LoadShader(STANDARD_BUMP_SHADER, L"Standard.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// Ani Bump Shader
	pEntryPoint[ST_VERTEX] = "StandardAniBumpVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "StandardBumpPS";
	pShader = LoadShader(STANDARD_ANI_BUMP_SHADER, L"Standard.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// SkyBox Shader
	pEntryPoint[ST_VERTEX] = "SkyVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "SkyPS";
	pShader = LoadShader("SkyShader", L"Sky.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// Effect Shader
	pEntryPoint[ST_VERTEX] = "EffectVS";
	pEntryPoint[ST_GEOMETRY] = "EffectGS";
	pEntryPoint[ST_PIXEL] = "EffectPS";
	pShader = LoadShader(EFFECT_SHADER, L"Effect.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// UI Shader
	pEntryPoint[ST_VERTEX] = "UIVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "UIPS";
	pShader = LoadShader(UI_SHADER, L"UI.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// Terrain Shader
	pEntryPoint[ST_VERTEX] = "TerrainVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "TerrainPS";
	pShader = LoadShader(TERRAIN_SHADER, L"Terrain.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// LightAcc Shader
	pEntryPoint[ST_VERTEX] = "LightAccVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "LightAccPS";
	pShader = LoadShader(LIGHT_ACC_SHADER, L"LightAcc.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// LightBlend Shader
	pEntryPoint[ST_VERTEX] = "LightBlendVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "LightBlendPS";
	pShader = LoadShader(LIGHT_BLEND_SHADER, L"LightBlend.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	// LightBlendTargetOutput Shader
	pEntryPoint[ST_VERTEX] = "LightBlendVS";
	pEntryPoint[ST_GEOMETRY] = NULL;
	pEntryPoint[ST_PIXEL] = "LightBlendOutputPS";
	pShader = LoadShader(LIGHT_BLEND_OUTPUT_SHADER, L"LightBlend.fx", pEntryPoint);
	SAFE_RELEASE(pShader);

	AddElement("POSITION", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("COLOR", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CreateInputLayout("ColorInputLayout", "StandardColorShader");

	AddElement("POSITION", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("NORMAL", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("TEXCOORD", 0, 8, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_INPUT_PER_VERTEX_DATA, 0);
	CreateInputLayout("TexNormalInputLayout", STANDARD_TEXNORMAL_SHADER);

	AddElement("POSITION", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);	
	AddElement("TEXCOORD", 0, 8, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CreateInputLayout("TexInputLayout", UI_SHADER);

	AddElement("POSITION", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("NORMAL", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("TEXCOORD", 0, 8, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("TANGENT", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("BINORMAL", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CreateInputLayout("BumpInputLayout", STANDARD_BUMP_SHADER);

	AddElement("POSITION", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("NORMAL", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("TEXCOORD", 0, 8, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("TANGENT", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("BINORMAL", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("BLENDWEIGHTS", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddElement("BLENDINDICES", 0, 16, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CreateInputLayout("AniBumpInputLayout", STANDARD_ANI_BUMP_SHADER);

	AddElement("POSITION", 0, 12, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_INPUT_PER_VERTEX_DATA, 0);
	CreateInputLayout("PosInputLayout", "SkyShader");

	// ������� ����
	CreateConstBuffer("Transform", sizeof(TRANSFORMCBUFFER), 0);

	// Material Const Buffer
	CreateConstBuffer("Material", sizeof(MATERIALINFO), 1);

	// Light Const Buffer
	CreateConstBuffer("Light", sizeof(LIGHTCBUFFER), 2);

	// ���÷����� ����
	CreateConstBuffer("ColliderColor", sizeof(COLCOLORCBUFFER), 10);

	// Effect Const Buffer
	CreateConstBuffer("BillBoard", sizeof(EFFECTCBUFFER), 11);

	// Animation2D Const Buffer
	CreateConstBuffer("Animation2D", sizeof(ANIMATION2DCBUFFER), 12);

	// Terrain Const Buffer
	CreateConstBuffer("Terrain", sizeof(TERRAINCBUFFER), 10);

	return true;
}

CShader * CShaderMgr::LoadShader(const string & _strKey, WCHAR * _pFileName, char* _pEntryPoint[ST_END], const string & _strPathKey)
{
	CShader* pShader = FindShader(_strKey);

	// ���̴� ������
	if (NULL != pShader)
	{
		// �ٷ� ����
		return pShader;
	}

	// ������ ����
	pShader = new CShader();

	if (false == pShader->LoadShader(_strKey, _pFileName, _pEntryPoint, _strPathKey))
	{
		SAFE_RELEASE(pShader);
		return NULL;
	}

	pShader->AddRef();

	// �ʿ� �μ�Ʈ �ϰ�
	m_mapShader.insert(make_pair(_strKey, pShader));

	// ����
	return pShader;
}

CShader * CShaderMgr::FindShader(const string & _strKey)
{
	unordered_map<string, CShader*>::iterator iter = m_mapShader.find(_strKey);

	if (m_mapShader.end() == iter)
	{
		// ã������ ���̴��� ���°��
		return NULL;
	}

	iter->second->AddRef();

	return iter->second;
}

void CShaderMgr::AddElement(char * _pSemanticName, int _iSemanticIndex, int _iSize, DXGI_FORMAT _eFromat, int _iInputSlot, D3D11_INPUT_CLASSIFICATION _eSlotClass, int _iInstanceStep)
{
	D3D11_INPUT_ELEMENT_DESC	tDesc = {};

	tDesc.SemanticName = _pSemanticName;
	tDesc.SemanticIndex = _iSemanticIndex;
	tDesc.Format = _eFromat;
	tDesc.InputSlot = _iInputSlot;
	tDesc.AlignedByteOffset = m_iOffsetSize;
	tDesc.InputSlotClass = _eSlotClass;
	tDesc.InstanceDataStepRate = _iInstanceStep;

	m_iOffsetSize += _iSize;

	m_vecElement.push_back(tDesc);
}

bool CShaderMgr::CreateInputLayout(const string & _strKey, const string & _strShaderKey)
{
	ID3D11InputLayout*	pLayout = FindInputLayout(_strKey);

	if (NULL != pLayout)
	{
		return false;
	}

	CShader*	pShader = FindShader(_strShaderKey);

	if (NULL == pShader)
	{
		return false;
	}
	
	if (FAILED(DEVICE->CreateInputLayout(&m_vecElement[0], m_vecElement.size(),
		pShader->GetVSCode(), pShader->GetVSCodeSize(), &pLayout)))
	{
		m_vecElement.clear();
		return false;
	}

	m_mapInputLayout.insert(make_pair(_strKey, pLayout));

	m_vecElement.clear();
	m_iOffsetSize = 0;

	SAFE_RELEASE(pShader);

	return true;
}

void CShaderMgr::SetInputLayout(const string & _strKey)
{
	ID3D11InputLayout*	pInput = FindInputLayout(_strKey);

	if (!pInput)
	{
		return;
	}

	CONTEXT->IASetInputLayout(pInput);
}

ID3D11InputLayout * CShaderMgr::FindInputLayout(const string & _strKey)
{
	unordered_map<string, ID3D11InputLayout*>::iterator	iter = m_mapInputLayout.find(_strKey);

	if (iter == m_mapInputLayout.end())
	{
		return NULL;
	}

	return iter->second;
}

bool CShaderMgr::CreateConstBuffer(const string & _strKey, int _iSize, int _iRegister)
{
	pCONSTBUFFER	pConstBuffer = FindConstBuffer(_strKey);

	if (NULL != pConstBuffer)
	{
		return false;
	}

	pConstBuffer = new CONSTBUFFER();
	pConstBuffer->iSize = _iSize;
	pConstBuffer->iRegister = _iRegister;

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = _iSize;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tDesc.MiscFlags = 0;
	tDesc.StructureByteStride = 0;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &pConstBuffer->pBuffer)))
	{
		SAFE_DELETE(pConstBuffer);
		return false;
	}

	m_mapCBuffer.insert(make_pair(_strKey, pConstBuffer));

	return true;
}

pCONSTBUFFER CShaderMgr::FindConstBuffer(const string _strKey)
{
	unordered_map<string, pCONSTBUFFER>::iterator iter = m_mapCBuffer.find(_strKey);

	if (m_mapCBuffer.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

void CShaderMgr::UpdateConstBuffer(const string & _strKey, void * _pData, int _iCut)
{
	pCONSTBUFFER	pConstBuffer = FindConstBuffer(_strKey);

	if (NULL == pConstBuffer)
	{
		assert(false);
		return;
	}

	D3D11_MAPPED_SUBRESOURCE	tMap = {};
	
	// ���� ������
	CONTEXT->Map(pConstBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

	memcpy(tMap.pData, _pData, pConstBuffer->iSize);

	CONTEXT->Unmap(pConstBuffer->pBuffer, 0);

	if (_iCut & CUT_VERTEX)
	{
		CONTEXT->VSSetConstantBuffers(pConstBuffer->iRegister, 1, &pConstBuffer->pBuffer);
	}

	if (_iCut & CUT_PIXEL)
	{
		CONTEXT->PSSetConstantBuffers(pConstBuffer->iRegister, 1, &pConstBuffer->pBuffer);
	}

	if (_iCut & CUT_GEOMETRY)
	{
		CONTEXT->GSSetConstantBuffers(pConstBuffer->iRegister, 1, &pConstBuffer->pBuffer);
	}
}

CShaderMgr::CShaderMgr()
{
}

CShaderMgr::~CShaderMgr()
{
	unordered_map<string, pCONSTBUFFER>::iterator	iter;
	unordered_map<string, pCONSTBUFFER>::iterator	iterEnd = m_mapCBuffer.end();

	for (iter = m_mapCBuffer.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pBuffer);
		SAFE_DELETE(iter->second);
	}

	m_mapCBuffer.clear();

	Safe_Release_Map(m_mapInputLayout);
	Safe_Release_Map(m_mapShader);
}