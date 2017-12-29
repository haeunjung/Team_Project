#include "Share.fx"

VS_COLOR_OUTPUT StandardColorVS(VS_COLOR_INPUT input)
{
	VS_COLOR_OUTPUT output = (VS_COLOR_OUTPUT)0;

    // ���Ϳ� ��� ��
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

    // ���Ϳ� ��� ��
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

	// clip �Լ��� ���ڰ� 0���� ���� ��� ���� �ȼ��� ����Ѵ�.
	// ���̹��ۿ��� ���̰��� ������ �ʴ´�.
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

    // �������� ���� ��� ������ ���� ���Ѵ�.
    float3 vNormal = input.vNormal;

    if (1 == g_iBump)
    {
        float4 vNormalMap = g_NormalTexture.Sample(g_NormalSampler, input.vUV);
        vNormal = vNormalMap.xyz * 2.0f - 1.0f;

        // �������͸� �̿��Ͽ� ź��Ʈ������ ������ ��������� ������ش�.
        float3x3 mat = float3x3(input.vTangent, input.vBinormal, input.vNormal);

        // Tangent, Binormal, Normal�� ���ؽ� ���̴����� View �������� ��ȯ�� ���������̴�.
        // �� �������� �̿��ؼ� ����� �Ʒ�ó�� �����Ѵ�.
        // Tx     Bx     Nx
        // Ty     By     Ny
        // Tz     Bz     Nz
        // �̷��� �����ϸ� ������� �ִ� ��� ���͸� ź��Ʈ �������� ��ȯ�ϴ� ����� ���������.
        // NormalMap�� ����� ���� ������ ź��Ʈ������ �����ϴ� �������̴�.
        // �� ���������� ��������� �ٲٷ��� �� ����� ������� �ʿ��ϴ�.
        // ��������� ������� ��ġ��İ� ���⶧����
        // Tx     Ty     Tz
        // Bx     By     Bz
        // Nx     Ny     Nz
        // ���� ���� ����̶�� �� �� �ִ�.
        // NormalMap���� �̾ƿ� ���͸� �� ��Ŀ� ���ϰ� �Ǹ� ź��Ʈ������ �ִ� ���� ������ ��������� ��ȯ�ȴ�.

        vNormal = mul(vNormal, mat);
        vNormal = normalize(vNormal);
    }

    _tagMaterial tMaterial = ComputeLight(vNormal, input.vViewPos, input.vUV);

    float4 vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV);

    // clip �Լ��� ���ڰ� 0���� ���� ��� ���� �ȼ��� ����Ѵ�.
	// ���̹��ۿ��� ���̰��� ������ �ʴ´�.
    if (vColor.a == 0.f)
        clip(-1);

    output.vTarget0.rgb = vColor.xyz * (tMaterial.vDiffuse.xyz + tMaterial.vAmbient.xyz) + tMaterial.vSpecular.xyz;
    output.vTarget0.a = vColor.a;

    return output;
}