#include "Share.fx"

cbuffer TerrainInfo : register(b10)
{
    float   g_fDetailLevel;
    int     g_iSplatCount;
    float3  g_vTerrainEmpty1;
}

Texture2DArray g_SplatTexArray : register(t11);
SamplerState g_SplatSmp : register(s11);

Texture2DArray g_SplatNTexArray : register(t12);
SamplerState g_SplatNSmp : register(s12);

Texture2DArray g_SplatSTexArray : register(t13);
SamplerState g_SplatSSmp : register(s13);

Texture2DArray g_AlphaTexArray : register(t14);
SamplerState g_AlphaSmp : register(s14);

_tagMaterial ComputeTerrainLight(float3 vNormal, float3 vViewPos, float2 vUV, float2 vSplatUV)
{
	// 조명 타입에 따라 조명의 방향을 구하고 뷰공간으로 변환한다.
    float3 vLightDir = (float3) 0;
    float3 vLightPos = (float3) 0;
    float fIntensity = (float) 1.0f;
    float fDist = (float) 0;
    float fSpot = (float) 1.0f;
	// 방향성 조명일 경우
    if (g_iLightType == 0)
    {
        vLightDir = mul(float4(g_vLightDir, 0.f), g_matView);
        vLightDir = normalize(vLightDir);
    }

	// 점 조명일 경우
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

	// Diffuse를 구한다.
    tMtrl.vDiffuse = g_vLightDiffuse * g_vMaterialDiffuse * max(0, dot(vNormal, vLightDir)) * fIntensity;
    tMtrl.vAmbient = g_vLightAmbient * g_vMaterialAmbient * fSpot;

	// 정반사광을 구한다.
	// 반사벡터를 구해준다.
    float3 vReflect = 2.f * vNormal * dot(vNormal, vLightDir) - vLightDir;
    vReflect = normalize(vReflect);

	// 정점에서 카메라를 향하는 벡터를 만든다.
    float3 vView = -normalize(vViewPos);

    float4 vSpecular = g_vMaterialSpecular;

    if (1 == g_iSpecular)
    {
        vSpecular = g_SpecularTexture.Sample(g_SpecularSampler, vUV);

        float3 vUVW;

        for (int i = 0; i < 8; i++)
        {
            vUVW = float3(vSplatUV, i);
            float fAlpha = g_AlphaTexArray.Sample(g_AlphaSmp, vUVW).r;

            if (0.0f == fAlpha)
            {
                continue;
            }

            vUVW = float3(vUV, i);
            vSpecular = vSpecular * (1.0f - fAlpha) + g_SplatSTexArray.Sample(g_SplatSmp, vUVW) * fAlpha;
        }
    }

    tMtrl.vSpecular = g_vLightSpecular * vSpecular * pow(max(0, dot(vReflect, vView)), g_fSpecularPower) * fIntensity;

    return tMtrl;
}

VS_BUMP_OUTPUT TerrainVS(VS_BUMP_INPUT input)
{
    VS_BUMP_OUTPUT output = (VS_BUMP_OUTPUT) 0;

    output.vProjPos = mul(float4(input.vPos, 1.0f), g_matWVP);
    output.vUV = input.vUV;
    output.vNormal = normalize(mul(float4(input.vNormal, 0.0f), g_matWV).xyz);
    //output.vViewPos = mul(float4(input.vPos, 1.0f), g_matWV);
    output.vPos = output.vProjPos;

    if (1 == g_iBump)
    {
        output.vTangent = normalize(mul(float4(input.vTangent, 0.0f), g_matWV).xyz);
        output.vBinormal = normalize(mul(float4(input.vBinormal, 0.0f), g_matWV).xyz);
    }

    return output;
}

PS_OUTPUT TerrainPS(VS_BUMP_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

	// 법선맵이 있을 경우 법선을 새로 구한다.
    float3 vNormal = input.vNormal;
    float2 vUV = input.vUV * g_fDetailLevel;
    float3 vUVW = float3(0.f, 0.f, 0.f);

    if (g_iBump == 1)
    {
        float4 vNormalMap = g_NormalTexture.Sample(g_NormalSampler, vUV);

        for (int i = 0; i < 8; i++)
        {
            vUVW = float3(input.vUV, i);
            float fAlpha = g_AlphaTexArray.Sample(g_AlphaSmp, vUVW).r;

            if (fAlpha == 0.f)
                continue;

            vUVW = float3(vUV, i);
            vNormalMap = vNormalMap * (1.f - fAlpha) +
				g_SplatNTexArray.Sample(g_SplatNSmp, vUVW) * fAlpha;
        }

        float3 vNormal = vNormalMap * 2.f - 1.f;
        vNormal = normalize(vNormal);

		// 기저벡터를 이용하여 탄젠트공간의 법선을 View 공간으로 만들어준다.
		// Tangent, Binormal, Normal은 버텍스 셰이더에서 View공간으로 변환된
		// 축정보들이다. 이 축정보를 이용해서 행렬을 아래처럼 구성하면
		// Tx Bx Nx
		// Ty By Ny
		// Tz Bz Nz
		// 이렇게 구성하면 뷰 공간에 있는 어느 벡터를 탄젠트 공간으로 변환하는
		// 행렬이 만들어진다. 
		// NormalMap에 저장된 법선 정보는 탄젠트공간에 존재하는 법선들이다.
		// 이 법선정보를 뷰공간으로 바꾸려면 위 행렬의 역행렬이 필요하다.
		// Tx Ty Tz
		// Bx By Bz
		// Nx Ny Nz
		// 그래서 NormalMap에서 뽑아온 벡터를 위 행렬에 곱하게 되면 탄젠트공간에 있던
		// 법선 정보가 뷰공간으로 변환된다.
        float3x3 mat = float3x3(input.vTangent, input.vBinormal, input.vNormal);

        vNormal = mul(vNormal, mat);
        vNormal = normalize(vNormal);
    }

	/*_tagMtrl	tMtrl = ComputeTerrainLight(vNormal, input.vViewPos, vUV,
		input.vUV, input.iSplatCount);*/

    float4 vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, vUV);

	// clip 함수는 인자가 0보다 작을 경우 현재 픽셀을 폐기한다.
	// 깊이버퍼에도 깊이값이 쓰이지 않는다.
    if (vColor.a == 0.f)
        clip(-1);

    for (int i = 0; i < 8; i++)
    {
        vUVW = float3(input.vUV, i);
        float fAlpha = g_AlphaTexArray.Sample(g_AlphaSmp, vUVW).r;

        if (fAlpha == 0.f)
            continue;

        vUVW = float3(vUV, i);
        vColor.rgb = vColor.rgb * (1.f - fAlpha) +
			g_SplatTexArray.Sample(g_SplatSmp, vUVW).rgb * fAlpha;
    }

    output.vTarget0.rgb = vColor.xyz; // *(tMtrl.vDif.xyz + tMtrl.vAmb.xyz) + tMtrl.vSpc.xyz;
    output.vTarget0.a = vColor.a;

    output.vTarget1.rgb = vNormal * 0.5f + 0.5f;
    output.vTarget1.a = 3.2f;

    float fDiffuse = ColorToPixel(float4(1.f, 1.f, 1.f, 1.f));
    float fAmbient = ColorToPixel(float4(1.f, 1.f, 1.f, 1.f));

    output.vTarget2.r = input.vProjPos.z / input.vProjPos.w;
    output.vTarget2.g = g_vMaterialDiffuse.r;
    output.vTarget2.b = g_vMaterialAmbient.r;
	//output.vTarget2.g = fDiffuse;
	//output.vTarget2.b = fAmbient;
    output.vTarget2.a = input.vProjPos.w;

    float4 vSpc = g_vMaterialSpecular;

    if (g_iSpecular == 1)
    {
        vSpc = g_SpecularTexture.Sample(g_SpecularSampler, vUV);

        float3 vUVW;

        for (int i = 0; i < 8; i++)
        {
            vUVW = float3(input.vUV, i);
            float fAlpha = g_AlphaTexArray.Sample(g_AlphaSmp, vUVW).r;

            if (fAlpha == 0.f)
                continue;

            vUVW = float3(vUV, i);
            vSpc = vSpc * (1.f - fAlpha) +
				g_SplatSTexArray.Sample(g_SplatSSmp, vUVW) * fAlpha;
        }
    }

    output.vTarget3 = vSpc;

    return output;
}