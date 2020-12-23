// PS에 Bind 한다는 것은 어떤 오브젝트인지 상관없이
// 알맞은 데이터가 입력된다면 
// 그것에 맞춰서 셰이더 파일에 적용되는 듯 하다
// PointLight에 정의된 PointLightCBuf 구조체는 아래 LightCBuf 구조체와 구조가 같다
// 하지만 PointLight의 mesh인 SolidSphere는 SolidPS에 바인드 되어있다
// 그럼에도 이 셰이더 파일과 통신하고 있다는 것....음..
cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;    // diffuseColor는 표면에 반사된 색상임
    float diffuseIntensity; // 반사된 분산광 색상의 세기
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    float padding[2];
};

Texture2D tex;

SamplerState splr;

float4 main(float3 worldPos : Position, float3 n : Normal, float2 tc : Texcoord) : SV_Target
{
	// fragment to light vector data
    // 빛 벡터(빛 벡터는 빛을 향한다)
	const float3 vToL = lightPos - worldPos;
    // 오브젝트와 광원 사이의 거리
	const float distToL = length(vToL);
    // direction, 단위벡터를 구함으로서 광원으로의 방향벡터를 구함
	const float3 dirToL = vToL / distToL;
	// attenuation, 빛의 세기가 거리에 따라 감소하는 방식을 제어하는 세 개의 감쇠 상수들
    // attConst(상수계수), attLin(1차계수), attQuad(2차계수)
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity, diffuseIntensity가 분산광 세기라면 max(0.0f, dot(dirToL, n)) 역시 마찬가지다...
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	// reflected light vector
    // 반사벡터 r = I - 2(n·I)n이다
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    // worldPos는 오브젝트의 위치다. ambient light는 반영되어있지 않다. 
    // 왜 diffuse color와 diffuseIntensity를 중복해서 더하는 걸까..?
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
    // final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}