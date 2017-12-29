#include "Share.fx"

VS_COLOR_OUTPUT StandardColorVS(VS_COLOR_INPUT input)
{
	VS_COLOR_OUTPUT output = (VS_COLOR_OUTPUT)0;

    // 벡터와 행렬 곱
    output.vPos = mul(float4(input.vPos, 1.0f), g_matWVP);
	output.vColor = input.vColor;

	return output;
}

PS_SINGLE_OUTPUT StandardColorPS(VS_COLOR_OUTPUT input)
{
	PS_SINGLE_OUTPUT output = (PS_SINGLE_OUTPUT)0;

	output.vTarget0 = input.vColor;

	return output;
}

VS_TEXTURENORMAL_OUTPUT StandardTextureNormalVS(VS_TEXTURENORMAL_INPUT input)
{
    VS_TEXTURENORMAL_OUTPUT output = (VS_TEXTURENORMAL_OUTPUT) 0;

    // 벡터와 행렬 곱
    output.vPos = mul(float4(input.vPos, 1.0f), g_matWVP);
    output.vUV = input.vUV;
    output.vNormal = mul(float4(input.vNormal, 0.0f), g_matWV);
    output.vViewPos = mul(float4(input.vPos, 1.0f), g_matWV);

    return output;
}

PS_SINGLE_OUTPUT StandardTextureNormalPS(VS_TEXTURENORMAL_OUTPUT input)
{
    PS_SINGLE_OUTPUT output = (PS_SINGLE_OUTPUT) 0;
        
    _tagMaterial tMtrl = ComputeLight(input.vNormal, input.vViewPos, input.vUV);

    float4 vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV);

	// clip 함수는 인자가 0보다 작을 경우 현재 픽셀을 폐기한다.
	// 깊이버퍼에도 깊이값이 쓰이지 않는다.
    if (vColor.a == 0.f)
        clip(-1);

    output.vTarget0.rgb = vColor.xyz * (tMtrl.vDiffuse.xyz + tMtrl.vAmbient.xyz) + tMtrl.vSpecular.xyz;
    output.vTarget0.a = vColor.a;

    return output;
}

VS_BUMP_OUTPUT StandardBumpVS(VS_BUMP_INPUT input)
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

PS_SINGLE_OUTPUT StandardBumpPS(VS_BUMP_OUTPUT input)
{
    PS_SINGLE_OUTPUT output = (PS_SINGLE_OUTPUT) 0;

    // 법선맵이 있을 경우 법선을 새로 구한다.
    float3 vNormal = input.vNormal;

    if (1 == g_iBump)
    {
        float4 vNormalMap = g_NormalTexture.Sample(g_NormalSampler, input.vUV);
        vNormal = vNormalMap.xyz * 2.0f - 1.0f;

        // 기저벡터를 이용하여 탄젠트공간의 법선을 뷰공간으로 만들어준다.
        float3x3 mat = float3x3(input.vTangent, input.vBinormal, input.vNormal);

        // Tangent, Binormal, Normal은 버텍스 셰이더에서 View 공간으로 변환된 축정도블이다.
        // 이 축정보를 이용해서 행렬을 아래처럼 구성한다.
        // Tx     Bx     Nx
        // Ty     By     Ny
        // Tz     Bz     Nz
        // 이렇게 구성하면 뷰공간에 있는 어느 벡터를 탄젠트 공간으로 변환하는 행렬이 만들어진다.
        // NormalMap에 저장된 법선 정보는 탄젠트공간에 존재하는 법선들이다.
        // 이 법선정보를 뷰공간으로 바꾸려면 위 행렬의 역행렬이 필요하다.
        // 직교행렬의 역행렬은 전치행렬과 같기때문에
        // Tx     Ty     Tz
        // Bx     By     Bz
        // Nx     Ny     Nz
        // 위와 같은 행렬이라고 볼 수 있다.
        // NormalMap에서 뽑아온 벡터를 위 행렬에 곱하게 되면 탄젠트공간에 있던 법선 정보가 뷰공간으로 변환된다.

        vNormal = mul(vNormal, mat);
        vNormal = normalize(vNormal);
    }

    _tagMaterial tMaterial = ComputeLight(vNormal, input.vViewPos, input.vUV);

    float4 vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV);

    // clip 함수는 인자가 0보다 작을 경우 현재 픽셀을 폐기한다.
	// 깊이버퍼에도 깊이값이 쓰이지 않는다.
    if (vColor.a == 0.f)
        clip(-1);

    output.vTarget0.rgb = vColor.xyz * (tMaterial.vDiffuse.xyz + tMaterial.vAmbient.xyz) + tMaterial.vSpecular.xyz;
    output.vTarget0.a = vColor.a;

    return output;
}