// TransformCbuf에서 보내는 Transforms 구조체이다.
cbuffer CBuf
{
	matrix modelView;
	matrix modelViewProj;
};

struct VSOut
{
    // Position, Normal은 InputLayOut에서 지정한 변수명과 같아야 한다.
	float3 worldPos : Position;
	float3 normal : Normal;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 n : Normal)
{
	VSOut vso;
	vso.worldPos = (float3)mul(float4(pos, 1.0f), modelView);
	vso.normal = mul(n, (float3x3)modelView);
	vso.pos = mul(float4(pos, 1.0f), modelViewProj);
	return vso;
}