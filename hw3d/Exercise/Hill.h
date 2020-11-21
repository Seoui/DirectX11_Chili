#pragma once
#include "DrawableBase.h"

class Hill : public DrawableBase<Hill>
{
public:
	Hill(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	float GetHeight(float x, float z) const;
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	bool SpawnControlWindow(int id, Graphics& gfx) noexcept;
private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
	// model transform
	DirectX::XMFLOAT3X3 mt;
};
