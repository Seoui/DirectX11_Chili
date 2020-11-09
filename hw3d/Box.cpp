#include "Box.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Cube.h"


Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 material)
	:
	TestObject(gfx, rng, adist, ddist, odist, rdist)
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		// 흠.. 정점에 색이 없다
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};
		// 모델의 정점과 정점의 인덱스 리스트를 구한다
		auto model = Cube::MakeIndependent<Vertex>();
		// 모델의 각 삼각형 면의 노멀벡터를 구한다
		model.SetNormalsIndependentFlat();

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },

		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else // 혹시나 실패할 경우를 대비하여 한번 더 도전!
	{
		SetIndexFromStatic();
	}

	// TransformCbuf 클래스의 역할: 변환 상수 버퍼
	// World -> View -> Projection 변환을
	// VS에 Bind한다. local-> World 변환은 Box 클래스에서 해주고 있음
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	// PS에 bind하기 위한 material 상수 버퍼
	struct PSMaterialConstant
	{
		// alinged16이 되어야 하므로 padding[3]이다.
		// specular light는 shininess을 결정한다
		dx::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} colorConst;
	// material은 XMFLOAT3타입이고 [0, 1] 실수범위의 랜덤색상임
	colorConst.color = material;
	// PSMaterialConstant 자료형으로 buffer를 만들고 PS에 bind 하고 있다
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, colorConst, 1u));
	// model deformation transform (per instance, not stored as bind)
	// Box 생성자에서 box의 월드 스케일을 설정한다.
	// z축 기준 스케일은 랜덤이고요
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, bdist(rng))
	);
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	// box의 world 좌표(스케일만 적용된) * box의 world좌표(RT + world좌표 기준 회전) 
	return dx::XMLoadFloat3x3(&mt) * TestObject::GetTransformXM();
}
