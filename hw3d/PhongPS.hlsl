// PS�� Bind �Ѵٴ� ���� � ������Ʈ���� �������
// �˸��� �����Ͱ� �Էµȴٸ� 
// �װͿ� ���缭 ���̴� ���Ͽ� ����Ǵ� �� �ϴ�
// PointLight�� ���ǵ� PointLightCBuf ����ü�� �Ʒ� LightCBuf ����ü�� ������ ����
// ������ PointLight�� mesh�� SolidSphere�� SolidPS�� ���ε� �Ǿ��ִ�
// �׷����� �� ���̴� ���ϰ� ����ϰ� �ִٴ� ��....��..
cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;    // diffuseColor�� ǥ�鿡 �ݻ�� ������
    float diffuseIntensity; // �ݻ�� �л걤 ������ ����
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
    // �� ����(�� ���ʹ� ���� ���Ѵ�)
	const float3 vToL = lightPos - worldPos;
    // ������Ʈ�� ���� ������ �Ÿ�
	const float distToL = length(vToL);
    // direction, �������͸� �������μ� ���������� ���⺤�͸� ����
	const float3 dirToL = vToL / distToL;
	// attenuation, ���� ���Ⱑ �Ÿ��� ���� �����ϴ� ����� �����ϴ� �� ���� ���� �����
    // attConst(������), attLin(1�����), attQuad(2�����)
	const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity, diffuseIntensity�� �л걤 ������ max(0.0f, dot(dirToL, n)) ���� ����������...
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));
	// reflected light vector
    // �ݻ纤�� r = I - 2(n��I)n�̴�
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
    // calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    // worldPos�� ������Ʈ�� ��ġ��. ambient light�� �ݿ��Ǿ����� �ʴ�. 
    // �� diffuse color�� diffuseIntensity�� �ߺ��ؼ� ���ϴ� �ɱ�..?
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), specularPower);
    // final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}