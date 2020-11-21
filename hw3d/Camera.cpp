#include "Camera.h"
#include "Imgui/imgui.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	/* 
		world 좌표기준 카메라의 위치다, 따라서 회전은
		world 좌표계의 x,y,z 축 기준으로 회전한다.
		그러니까 카메라는 마치 world의 원점 
		중심으로 공전하는것 처럼 움직이게 된다.
		벡터를 변환매트릭스에 의해 변환된 벡터를 반환
	*/
	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	);
	// 리턴 view matrix
	/*
		XMMatrixLookAtLH(카메라 위치, 바라보는 대상 위치, world 업벡터)
		XMMatrixRotationRollPitchYaw를 곱하는 이유는
		view matrix는 카메라의 local space임. 따라서
		local space 기준으로 회전을 곱하면 camera 좌표계 기준으로 
		회전하게 되므로 자기 자신이 회전하게 된다.
	*/
	return dx::XMMatrixLookAtLH(
		pos, dx::XMVectorZero(),
		dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * dx::XMMatrixRotationRollPitchYaw(
		pitch, -yaw, roll
	);
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("R", &r, 0.0f, 80.0f, "%.1f");
		ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
		ImGui::SliderAngle("Phi", &phi, -89.0f, 89.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	r = 20.0f;
	theta = 0.0f;
	phi = 0.0f;
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}
