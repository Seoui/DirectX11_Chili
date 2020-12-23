cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};

cbuffer TransformCBuf
{
	matrix modelView;
	matrix modelViewProj;
};

Texture2D tex;
Texture2D nmap;

SamplerState splr;

float4 main(float3 worldPos : Position, float3 n : Normal, float2 tc : Texcoord) : SV_Target
{
    // sample normal from map if normal mapping enabled
    if (normalMapEnabled)
    {
        const float3 normalSample = nmap.Sample(splr, tc).xyz;
        // 색의 범위가 0.0~1.0 이니까 두 배하면 0.0~2.0됨 여기서 -1을 빼면 -1.0~ 1.0
        n.x = normalSample.x * 2.0f - 1.0f;
        n.y = -normalSample.y * 2.0f + 1.0f;
        n.z = -normalSample.z;
		n = mul(n, (float3x3)modelView);
    }
    // fragment to light vector data
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
    // attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	/*
	// reflected light vector
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;*/
	const float3 r = reflect(vToL, n);
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(r), normalize(worldPos))), specularPower);
	// final color 
	// 텍스처를 재질/조명 시스템과 통합할 때에는, 텍스처 색상을 주변광과 분산광으로 변조(modulation)하되
	// 반영광으로는 변조하지 않도록 하는 것이 일반적이다(그런 방식을 흔히 '변조 후 가산[modulate with late add]'이라고 부른다).
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}