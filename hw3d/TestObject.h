#pragma once
#include "DrawableBase.h"
#include "ChiliMath.h"

template<class T>
class TestObject : public DrawableBase<T>
{
public:
	TestObject(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist)
		:
		r(rdist(rng)),
		droll(ddist(rng)),
		dpitch(ddist(rng)),
		dyaw(ddist(rng)),
		dphi(odist(rng)),
		dtheta(odist(rng)),
		dchi(odist(rng)),
		chi(adist(rng)),
		theta(adist(rng)),
		phi(adist(rng))
	{}
	void Update(float dt) noexcept
	{
		roll = wrap_angle(roll + droll * dt);
		pitch = wrap_angle(pitch + dpitch * dt);
		yaw = wrap_angle(yaw + dyaw * dt);
		theta = wrap_angle(theta + dtheta * dt);
		phi = wrap_angle(phi + dphi * dt);
		chi = wrap_angle(chi + dchi * dt);
	}
	/* 
		(회전 변환 * 이동 변환) * 회전 변환 
		이게 얼핏보면 회전을 한번 그냥 한 것 같지만 아니야!!!
		앞서 스케일 변환은 이미 이루어진 상태이고, W = SRT 순서에 따라
		회전 변환과 이동변환을 한다. 이후에는 W변환이고 이것은 
		로컬 좌표계가 월드 좌표계로 바뀌었다는 것을 의미한다
		여기서 한번 더 회전 변환을 하면 월드 좌표계 기준으로 회전하는 것이다.
		따라서 뒤에 회전은 월드의 원점을 기준으로 공전하듯이 회전한다.
	*/
	DirectX::XMMATRIX GetTransformXM() const noexcept
	{
		namespace dx = DirectX;
		return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
			dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}
protected:
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
};