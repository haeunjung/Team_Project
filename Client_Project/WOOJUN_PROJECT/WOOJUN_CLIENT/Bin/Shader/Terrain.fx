#include "Share.fx"

cbuffer TerrainInfo : register(b10)
{
    float   g_fDetailLevel;
    int     g_iSplatCount;
    float3  g_vTerrainEmpty1;
}

Texture2DArray g_SplatTexArray : register(t11);
SamplerState g_SplatSampler : register(s11);

Texture2DArray g_SplatNrmTexArray : register(t12);
SamplerState g_SplatNrmSampler : register(s12);

Texture2DArray g_SplatSpcTexArray : register(t13);
SamplerState g_SplatSpcrSampler : register(s13);

Texture2DArray g_AlphaTexArray : register(t14);
SamplerState g_AlphaSampler : register(s14);

_tagMaterial ComputeTerrainLight(float3 vNormal, float3 vViewPos, float2 vUV, float2 vSplatUV)
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
        vSpecular = g_SpecularTexture.Sample(g_SpecularSampler, vUV);

        float3 vUVW;

        for (int i = 0; i < 8; i++)
        {
            vUVW = float3(vSplatUV, i);
            float fAlpha = g_AlphaTexArray.Sample(g_AlphaSampler, vUVW).r;

            if (0.0f == fAlpha)
            {
                continue;
            }

            vUVW = float3(vUV, i);
            vSpecular = vSpecular * (1.0f - fAlpha) + g_SplatSpcTexArray.Sample(g_SplatSpcrSampler, vUVW) * fAlpha;
        }
    }

    tMtrl.vSpecular = g_vLightSpecular * vSpecular * pow(max(0, dot(vReflect, vView)), g_fSpecularPower) * fIntensity;

    return tMtrl;
}

VS_BUMP_OUTPUT TerrainVS(VS_BUMP_INPUT input)
{
    VS_BUMP_OUTPUT output = (VS_BUMP_OUTPUT) 0;

    output.vPos = mul(float4(input.vPos, 1.0f), g_matWVP);
    output.vUV = input.vUV;
    output.vNormal = normalize(mul(float4(input.vNormal, 0.0f), g_matWV).xyz);
    output.vViewPos = mul(float4(input.vPos, 1.0f), g_matWV);

    if (1 == g_iBump)
    {
        output.vTangent = normalize(mul(float4(input.vTangent, 0.0f), g_matWV).xyz);
        output.vBinormal = normalize(mul(float4(input.vBinormal, 0.0f), g_matWV).xyz);
    }

    return output;
}

PS_SINGLE_OUTPUT TerrainPS(VS_BUMP_OUTPUT input)
{
    PS_SINGLE_OUTPUT output = (PS_SINGLE_OUTPUT) 0;

    // �������� ���� ��� ������ ���� ���Ѵ�.
    float3 vNormal = input.vNormal;
    float2 vUV = input.vUV * g_fDetailLevel;
    float3 vUVW = float3(0.0f, 0.0f, 0.0f);

    if (1 == g_iBump)
    {
        float4 vNormalMap = g_NormalTexture.Sample(g_NormalSampler, vUV);
      
        for (int i = 0; i < 8; i++)
        {
            vUVW = float3(input.vUV, i);
            float fAlpha = g_AlphaTexArray.Sample(g_AlphaSampler, vUVW).r;

            if (0.0f == fAlpha)
            {
                continue;
            }

            vUVW = float3(vUV, i);
            vNormalMap = vNormalMap * (1.0f - fAlpha) + g_SplatNrmTexArray.Sample(g_SplatNrmSampler, vUVW) * fAlpha;
        }

        float3 vNormal = vNormalMap * 2.0f - 1.0f;
        vNormal = normalize(vNormal);

        float3x3 mat = float3x3(input.vTangent, input.vBinormal, input.vNormal);

        vNormal = mul(vNormal, mat);
        vNormal = normalize(vNormal);
    }

    _tagMaterial tMaterial = ComputeTerrainLight(vNormal, input.vViewPos, vUV, input.vUV);

    float4 vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, vUV);

    // clip �Լ��� ���ڰ� 0���� ���� ��� ���� �ȼ��� ����Ѵ�.
	// ���̹��ۿ��� ���̰��� ������ �ʴ´�.
    if (vColor.a == 0.f)
        clip(-1);

    [unroll]
    for (int i = 0; i < 4; i++)
    {
        vUVW = float3(input.vUV, i);
        float fAlpha = g_AlphaTexArray.Sample(g_AlphaSampler, vUVW).r;

        if (0.0f == fAlpha)
        {
            continue;
        }

        vUVW = float3(vUV, i);
        vColor.rgb = vColor.rgb * (1.0f - fAlpha) + g_SplatTexArray.Sample(g_SplatSampler, vUVW).rgb * fAlpha;

    }

    output.vTarget0.rgb = vColor.xyz * (tMaterial.vDiffuse.xyz + tMaterial.vAmbient.xyz) + tMaterial.vSpecular.xyz;
    output.vTarget0.a = vColor.a;

    return output;
}