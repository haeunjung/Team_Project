#include "Share.fx"

SamplerState g_GBufferSampler : register(s11);

Texture2D g_GBufferAlbedo : register(t11);
Texture2D g_GBufferNormal : register(t12);
Texture2D g_GBufferDepth : register(t13);
Texture2D g_GBufferSpecular : register(t14);

static const float2 vPos[4] =
{
    float2(-1.f, 1.f),
	float2(1.f, 1.f),
	float2(-1.f, -1.f),
	float2(1.f, -1.f)
};

static const float2 vUV[4] =
{
    float2(0.f, 0.f),
	float2(1.f, 0.f),
	float2(0.f, 1.f),
	float2(1.f, 1.f)
};

VS_TEX_OUTPUT LightAccVS(uint iVertexID : SV_VertexID)
{
    VS_TEX_OUTPUT output = (VS_TEX_OUTPUT) 0;

    output.vPos = float4(vPos[iVertexID], 0.f, 1.f);
    output.vUV = vUV[iVertexID];

    return output;
}

_tagMaterial ComputeAccLight(float3 vNormal, float3 vViewPos, float2 vUV, float fSpecularPower, float4 vMtrlDif, float4 vMtrlAmb)
{
    _tagMaterial tMtrl = (_tagMaterial) 0;

	// 조명 타입에 따라 조명의 방향을 구하고 뷰공간으로 변환한다.
    float3 vLightDir = (float3) 0;
    float3 vLightPos = (float3) 0;
    float fAtt = (float) 1.0f;
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
       // 조명의 위치를 뷰공간으로 바꾼다.
        float3 vLightPos = mul(float4(g_vLightPos, 1.0f), g_matView);
        vLightDir = vLightPos - vViewPos;

        fDist = length(vLightDir);

        vLightDir = normalize(vLightDir);

        if (fDist > g_fLightRange)
            return tMtrl;
        else
        {
            fAtt = 1.0f / dot(g_vAttenuation.xyz, float3(1.0f, fDist, fDist * fDist));
        }
    }

	// Spot
    if (g_iLightType == 2)
    {
       // 조명의 위치를 뷰공간으로 바꾼다.
        float3 vLightPos = mul(float4(g_vLightPos, 1.0f), g_matView);
        vLightDir = vLightPos - vViewPos;

        fDist = length(vLightDir);

        vLightDir = normalize(vLightDir);

        if (fDist > g_fLightRange)
            return tMtrl;
        else
        {
            fSpot = pow(max(dot(-vLightDir, g_vLightDir), 0.0f), g_fSpot);

            fAtt = fSpot / dot(g_vAttenuation.xyz, float3(1.0f, fDist, fDist * fDist));
        }
    }

    if (g_iLightType == 3)
    {
        return tMtrl;
    }

	// Diffuse를 구한다.
        tMtrl.vDiffuse = g_vLightDiffuse * vMtrlDif * max(0, dot(vNormal, vLightDir)) * fAtt;
    tMtrl.vAmbient = g_vLightAmbient * vMtrlAmb * fAtt;

	// 정반사광을 구한다.
	// 반사벡터를 구해준다.
    float3 vReflect = 2.f * vNormal * dot(vNormal, vLightDir) - vLightDir;
    vReflect = normalize(vReflect);

	// 정점에서 카메라를 향하는 벡터를 만든다.
    float3 vView = -normalize(vViewPos);

    float4 vSpecular;

    vSpecular.rgb = g_GBufferSpecular.Sample(g_GBufferSampler, vUV).rgb;
    vSpecular.a = 1.f;

    tMtrl.vSpecular = g_vLightSpecular * vSpecular * pow(max(0, dot(vReflect, vView)), g_fSpecularPower) * fAtt;

    return tMtrl;
}

PS_OUTPUT LightAccPS(VS_TEX_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    float4 vNormalCol = g_GBufferNormal.Sample(g_GBufferSampler, input.vUV);

    if (0.f == vNormalCol.a)
    {
        clip(-1);
    }

    float3 vNormal = vNormalCol * 2.f - 1.f;

    float4 vDepth = g_GBufferDepth.Sample(g_GBufferSampler, input.vUV);

    // uv좌표를 -1 ~ 1 사이로 만든다.
    float4 vProjPos;
    vProjPos.x = (input.vUV.x * 2.f - 1.f) * vDepth.w;
    vProjPos.y = (input.vUV.y * -2.f + 1.f) * vDepth.w;
    vProjPos.z = vDepth.x * vDepth.w;
    vProjPos.w = vDepth.w;

    float3 vViewPos = mul(vProjPos, g_matInvProj).xyz;

    float4 vDif = (float4) vDepth.y;
    float4 vAmb = (float4) vDepth.z;

    _tagMaterial tMtrl = ComputeAccLight(vNormal, vViewPos, input.vUV, vNormalCol.a, vDif, vAmb);

    output.vTarget0 = tMtrl.vDiffuse + tMtrl.vAmbient;
    output.vTarget1 = tMtrl.vSpecular;

    return output;
}