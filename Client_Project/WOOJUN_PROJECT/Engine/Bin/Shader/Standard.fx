#include "Share.fx"

VS_COLOR_OUTPUT StandardColorVS(VS_COLOR_INPUT input)
{
	VS_COLOR_OUTPUT output = (VS_COLOR_OUTPUT)0;

    // 벡터와 행렬 곱
    output.vPos = mul(float4(input.vPos, 1.0f), g_matWVP);
	output.vColor = input.vColor;

	return output;
}

PS_OUTPUT StandardColorPS(VS_COLOR_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

	output.vTarget0 = input.vColor;

	return output;
}

VS_TEXTURENORMAL_OUTPUT StandardTextureNormalVS(VS_TEXTURENORMAL_INPUT input)
{
    VS_TEXTURENORMAL_OUTPUT output = (VS_TEXTURENORMAL_OUTPUT) 0;

    // 벡터와 행렬 곱
    output.vProjPos = mul(float4(input.vPos, 1.0f), g_matWVP);
    output.vUV = input.vUV;
    output.vNormal = mul(float4(input.vNormal, 0.0f), g_matWV);
    //output.vViewPos = mul(float4(input.vPos, 1.0f), g_matWV);
    output.vPos = output.vProjPos;

    return output;
}

PS_OUTPUT StandardTextureNormalPS(VS_TEXTURENORMAL_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;
        
    //_tagMaterial tMtrl = ComputeLight(input.vNormal, input.vViewPos, input.vUV);

    float4 vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV);

	// clip 함수는 인자가 0보다 작을 경우 현재 픽셀을 폐기한다.
	// 깊이버퍼에도 깊이값이 쓰이지 않는다.
    if (vColor.a == 0.f)
        clip(-1);

    output.vTarget0.rgb = vColor.xyz;// * (tMtrl.vDiffuse.xyz + tMtrl.vAmbient.xyz) + tMtrl.vSpecular.xyz;
    output.vTarget0.a = vColor.a;

    output.vTarget1.rgb = input.vNormal * 0.5f + 0.5f;
    output.vTarget1.a = 1.0f;

    output.vTarget2.rgb = (float3)(input.vProjPos.z / input.vProjPos.w);
    output.vTarget2.a = 1.f;

    float4 vSpc = g_vMaterialSpecular;

    if (g_iSpecular == 1)
    {
        vSpc = g_SpecularTexture.Sample(g_SpecularSampler, input.vUV);
    }

    output.vTarget3 = vSpc;

    return output;
}

VS_BUMP_OUTPUT StandardBumpVS(VS_BUMP_INPUT input)
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

VS_BUMP_OUTPUT StandardAniBumpVS(VS_ANI_BUMP_INPUT input)
{
    VS_BUMP_OUTPUT output = (VS_BUMP_OUTPUT) 0;

    // 스키닝
    _tagSkinning    tSkinning = Skinning(input.vPos, input.vNormal, input.vTangent, input.vBinormal, input.vWeights, input.vIndices);
    
    output.vProjPos = mul(float4(tSkinning.vPos, 1.0f), g_matWVP);
    output.vUV = input.vUV;
    output.vNormal = normalize(mul(float4(tSkinning.vNormal, 0.0f), g_matWV).xyz);
    //output.vViewPos = mul(float4(tSkinning.vPos, 1.0f), g_matWV);
    output.vPos = output.vProjPos;

    if (1 == g_iBump)
    {
        output.vTangent = normalize(mul(float4(tSkinning.vTangent, 0.0f), g_matWV).xyz);
        output.vBinormal = normalize(mul(float4(tSkinning.vBinormal, 0.0f), g_matWV).xyz);
    }

    return output;
}

PS_OUTPUT StandardBumpPS(VS_BUMP_OUTPUT input)
{
    PS_OUTPUT output = (PS_OUTPUT) 0;

    // 법선맵이 있을 경우 법선을 새로 구한다.
    float3 vNormal = input.vNormal;

    if (1 == g_iBump)
    {
        vNormal = ComputeNormal(input.vTangent, input.vBinormal, input.vNormal, input.vUV);
    }

    //_tagMaterial tMaterial = ComputeLight(vNormal, input.vViewPos, input.vUV);

    float4 vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV);

    // clip 함수는 인자가 0보다 작을 경우 현재 픽셀을 폐기한다.
	// 깊이버퍼에도 깊이값이 쓰이지 않는다.
    if (vColor.a == 0.f)
        clip(-1);

    output.vTarget0.rgb = vColor.xyz;// * (tMaterial.vDiffuse.xyz + tMaterial.vAmbient.xyz) + tMaterial.vSpecular.xyz;
    output.vTarget0.a = vColor.a;

    output.vTarget1.rgb = vNormal * 0.5f + 0.5f;
    output.vTarget1.a = 1.0f;

    output.vTarget2.rgb = (float3)(input.vProjPos.z / input.vProjPos.w);
    output.vTarget2.a = 1.f;

    float4 vSpc = g_vMaterialSpecular;

    if (g_iSpecular == 1)
    {
        vSpc = g_SpecularTexture.Sample(g_SpecularSampler, input.vUV);
    }

    output.vTarget3 = vSpc;

    return output;
}