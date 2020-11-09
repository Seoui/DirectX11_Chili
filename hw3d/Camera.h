#pragma once
#include "Graphics.h"
/*
	r		: �Ÿ�
	theta	: ����þ߰�
	phi		: �����þ߰�
*/
class Camera
{
public:
	// view matrix�� ������
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