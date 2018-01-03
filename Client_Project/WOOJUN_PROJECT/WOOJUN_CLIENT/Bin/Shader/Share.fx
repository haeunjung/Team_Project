// Color Vertex Input Structure
struct VS_COLOR_INPUT
{
	float3 vPos		: POSITION;
	float4 vColor	: COLOR;
};

// Color Vertex Output Structure
struct VS_COLOR_OUTPUT
{
	float4 vPos		: SV_POSITION;		
	// SV�� System Value�� ���ڷ� 
	// System Value�� �Ǹ� �ǵ帮�� �ȵȴ�.
	float4 vColor	: COLOR;
};

// Texture Normal Vertex
struct VS_TEXTURENORMAL_INPUT
{
    float3 vPos     : POSITION;
    float3 vNormal  : NORMAL;
    float2 vUV      : TEXCOORD;
};

struct VS_TEXTURENORMAL_OUTPUT
{
    float4 vPos     : SV_POSITION;
    float3 vNormal  : NORMAL;
    float2 vUV      : TEXCOORD;
    float3 vViewPos : POSITION;
};

// Texture Bump Vertex
struct VS_BUMP_INPUT
{
    float3 vPos      : POSITION;
    float3 vNormal   : NORMAL;
    float2 vUV       : TEXCOORD;
    float3 vTangent  : TANGENT;
    float3 vBinormal : BINORMAL;
};

struct VS_BUMP_OUTPUT
{
    float4 vPos      : SV_POSITION;
    float3 vNormal   : NORMAL;
    float2 vUV       : TEXCOORD;
    float3 vViewPos  : POSITION;
    float3 vTangent  : TANGENT;
    float3 vBinormal : BINORMAL;
};

// Single Target Output Structure
struct PS_SINGLE_OUTPUT
{
	float4 vTarget0		: SV_TARGET;
};

// �������
// ���̴� �ܺο��� ���� �Ѱ��ٶ� ���
cbuffer Transform : register(b0)
{
    matrix  g_matWorld;
    matrix  g_matView;
    matrix  g_matProj;
    matrix  g_matWV;
    matrix  g_matWVP;
    matrix  g_matVP;

    float3  g_vPivot;
    float   g_Empty1;
    float3  g_vMeshSize;
    float   g_Empty2;
    float3  g_vMeshMin;
    float   g_Empty3;
    float3  g_vMeshMax;
    float   g_Empty4;
}

cbuffer Material : register(b1)
{
    float4 g_vMaterialDiffuse;
    float4 g_vMaterialAmbient;
    float4 g_vMaterialSpecular;
    float4 g_vMaterialEmissive; 

    int    g_iBump;
    int    g_iSpecular;
    float  g_fSpecularPower;
    int    g_iMaterialEmpty;
}

cbuffer Light : register(b2)
{
    float4  g_vLightDiffuse;
    float4  g_vLightAmbient;
    float4  g_vLightSpecular;
    int     g_iLightType;
    float3  g_vLightDir;
    float3  g_vLightPos;
    float   g_fLightRange;
    float3  g_vAttenuation;
    float   g_fSpot;
}

// Diffuse Texture
Texture2D    g_DiffuseTexture : register(t0);
SamplerState g_DiffuseSampler : register(s0);

// Normal Texture
Texture2D    g_NormalTexture : register(t1);
SamplerState g_NormalSampler : register(s1);

// Specular Texture
Texture2D    g_SpecularTexture : register(t2);
SamplerState g_SpecularSampler : register(s2);

struct _tagMaterial
{
    float4  vDiffuse;
    float4  vAmbient;
    float4  vSpecular;
};

_tagMaterial ComputeLight(float3 vNormal, float3 vViewPos, float2 vUV)
{
	// ���� Ÿ�Կ� ���� ������ ������ ���ϰ� ��������� ��ȯ�Ѵ�.
    float3 vLightDir = (float3) 0;
    float3 vLightPos = (float3) 0;
    float fIntensity = (float) 1.0f;    
    float fDist = (float) 0;
    float fSpot = (float) 1.0f;
	// ���⼺ ������ ���
    if (g_iLightType == 0)
    {
        vLightDir = mul(float4(g_vLightDir, 0.f), g_matView);
        vLightDir = normalize(vLightDir);
    }

	// �� ������ ���
    if (g_iLightType == 1)
    {
        vLightPos = mul(float4(g_vLightPos, 1.0f), g_matView);
        vLightDir = vLightPos - vViewPos;
        fDist = length(vLightDir);
        
        if (fDist > g_fLightRange)
        {
            vLightDir = float4(0.0f, 0.0f, 0.0f, 0.0f);
        }

        vLightDir /= fDist;

        fIntensity = 1.0f / dot(g_vAttenuation, float3(1.0f, fDist, fDist * fDist));
    }

	// Spot
    if (g_iLightType == 2)
    {
        vLightPos = mul(float4(g_vLightPos, 1.0f), g_matView);
        vLightDir = vLightPos - vViewPos;
        fDist = length(vLightDir);
        
        if (fDist > g_fLightRange)
        {
            vLightDir = float4(0.0f, 0.0f, 0.0f, 0.0f);
        }

        vLightDir /= fDist;        
        float3 vLight = -mul(float4(g_vLightDir, 0.0f), g_matView);

        fSpot = pow(max(dot(vLightDir, vLight), 0.0f), g_fSpot);
        fIntensity = fSpot / dot(g_vAttenuation, float3(1.0f, fDist, fDist * fDist));
    }

    _tagMaterial tMtrl = (_tagMaterial) 0;

	// Diffuse�� ���Ѵ�.
    tMtrl.vDiffuse = g_vLightDiffuse * g_vMaterialDiffuse * max(0, dot(vNormal, vLightDir)) * fIntensity;
    tMtrl.vAmbient = g_vLightAmbient * g_vMaterialAmbient * fSpot;

	// ���ݻ籤�� ���Ѵ�.
	// �ݻ纤�͸� �����ش�.
    float3 vReflect = 2.f * vNormal * dot(vNormal, vLightDir) - vLightDir;
    vReflect = normalize(vReflect);

	// �������� ī�޶� ���ϴ� ���͸� �����.
    float3 vView = -normalize(vViewPos);

    float4 vSpecular = g_vMaterialSpecular;

    if (1 == g_iSpecular)
    {
        vSpecular.rgb = g_SpecularTexture.Sample(g_SpecularSampler, vUV).rgb;
    }

    tMtrl.vSpecular = g_vLightSpecular * vSpecular * pow(max(0, dot(vReflect, vView)), g_fSpecularPower) * fIntensity;

    return tMtrl;
}