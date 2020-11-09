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
	-	Diffuse Light(분산광):
			표면에 한 점을 빛이 때리면 광선들이 다양한 무작위 방향들로 흩어진다
		공식: 빛의 세기(kd) x (입사광(ld) ⓧ 분산광 재질 색상(md))
		표면에 들어오는 입사광과 분산광 재질 색상을 ⓧ 연산을 하면 반사된
		분산광의 색상이 주어진다.
		kd = max(L·n, 0) 이고 스칼라 값이다. 빛의 세기를 의미하고
		이것을 분산광의 색상에 곱하면 반사된 분산광을 구할 수 있다.

		
	-	Ambient Light(주변광):
			광원에서 간접적으로 표면에 도달한 빛.
			간접적으로라는 것은 다른 표면에 적어도 한번 반사되었다는 뜻
		공식: diffuse light + ambient light, ambient light = 입사광(la) ⓧ 주변광 재질 색상(ma)

	-	Phong lighting(퐁 조명) 또는 per pixel lighting(픽셀별 조명) :
		법선의 보간과 조명 계산을 픽셀별로 수행하는 것

	-	Attenuation : 빛의 세기가 거리에 따라 감소하는 방식을 제어하는 감쇠 상수
	
	-	alingas(16) : HLSL에서는 data 멤버들이 4차원 벡터 단위로 채워 넣되
		하나의 멤버가 두 개의 4차원 벡터에 걸쳐 나누어지면 안된다는 제약이 있다.
		FLOAT3는 -> (x, y, z, padding) 형식으로 채워져야 한다.
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

