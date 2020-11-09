#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
	/*
	-	Diffuse Light(�л걤):
			ǥ�鿡 �� ���� ���� ������ �������� �پ��� ������ ������ �������
		����: ���� ����(kd) x (�Ի籤(ld) �� �л걤 ���� ����(md))
		ǥ�鿡 ������ �Ի籤�� �л걤 ���� ������ �� ������ �ϸ� �ݻ��
		�л걤�� ������ �־�����.
		kd = max(L��n, 0) �̰� ��Į�� ���̴�. ���� ���⸦ �ǹ��ϰ�
		�̰��� �л걤�� ���� ���ϸ� �ݻ�� �л걤�� ���� �� �ִ�.

		
	-	Ambient Light(�ֺ���):
			�������� ���������� ǥ�鿡 ������ ��.
			���������ζ�� ���� �ٸ� ǥ�鿡 ��� �ѹ� �ݻ�Ǿ��ٴ� ��
		����: diffuse light + ambient light, ambient light = �Ի籤(la) �� �ֺ��� ���� ����(ma)

	-	Phong lighting(�� ����) �Ǵ� per pixel lighting(�ȼ��� ����) :
		������ ������ ���� ����� �ȼ����� �����ϴ� ��

	-	Attenuation : ���� ���Ⱑ �Ÿ��� ���� �����ϴ� ����� �����ϴ� ���� ���
	
	-	alingas(16) : HLSL������ data ������� 4���� ���� ������ ä�� �ֵ�
		�ϳ��� ����� �� ���� 4���� ���Ϳ� ���� ���������� �ȵȴٴ� ������ �ִ�.
		FLOAT3�� -> (x, y, z, padding) �������� ä������ �Ѵ�.
	*/
	struct PointLightCBuf
	{
		alignas(16) DirectX::XMFLOAT3 pos;
		alignas(16) DirectX::XMFLOAT3 ambient;
		alignas(16) DirectX::XMFLOAT3 diffuseColor;
		float diffuseIntensity;
		float attConst;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};

