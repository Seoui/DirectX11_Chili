#include "Hill.h"
#include "BindableBase.h"
#include "GraphicsThrowMacros.h"
#include "Grid.h"
#include "ChiliMath.h"
#include "Imgui/imgui.h"

Hill::Hill(Graphics & gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dtheta(odist(rng)),
	dphi(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	namespace dx = DirectX;

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT4 color;
		};
		auto model = Grid::Make<Vertex>(5.0f, 5.0f, 5, 5);

		for (size_t i = 0; i < model.vertices.size(); ++i)
		{
			dx::XMFLOAT3 p = model.vertices[i].pos;
			p.y = GetHeight(p.x, p.z);
			model.vertices[i].pos.y = p.y;

			if (p.y < -10.0f)
			{
				model.vertices[i].color = { 1.0f, 0.96f, 0.62f, 1.0f };
			}
			else if (p.y < 5.0f)
			{
				model.vertices[i].color = { 0.48f, 0.77f, 0.46f, 1.0f };
			}
			else if (p.y < 12.0f)
			{
				model.vertices[i].color = { 0.1f, 0.48f, 0.19f, 1.0f };
			}
			else if (p.y < 20.0f)
			{
				model.vertices[i].color = { 0.45f, 0.39f, 0.34f, 1.0f };
			}
			else
			{
				model.vertices[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			}
		}

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"ColorBlendVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"ColorBlendPS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Color",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling(1.0f, 1.0f, 1.0f)
	);

}

float Hill::GetHeight(float x, float z) const
{
	return 0.3f * (z*sinf(0.1f*x) + x * cosf(0.1f*z));
}

void Hill::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Hill::GetTransformXM() const noexcept
{
	namespace dx = DirectX;
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
		dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
		/*
		dx::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
		dx::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) *
		dx::XMMatrixTranslation(0.0f, 0.0f, 10.0f);
		*/
}

bool Hill::SpawnControlWindow(int id, Graphics & gfx) noexcept
{
	using namespace std::string_literals;
	
	bool open = true;
	if (ImGui::Begin("Hill"s.c_str()), &open)
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -180.0f, 180.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
	}

	return open;
}
