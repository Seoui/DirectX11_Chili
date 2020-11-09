#pragma once
#include "Graphics.h"
/*
	r		: 거리
	theta	: 수평시야각
	phi		: 수직시야각
*/
class Camera
{
public:
	// view matrix를 리턴함
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
private:
	float r = 20.0f;
	float theta = 0.0f;
	float phi = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};