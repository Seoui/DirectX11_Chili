#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

// 변환 상수 버퍼
class TransformCbuf : public Bindable
{
private:
	struct Transforms
	{
		// homogeneous clip space(동차절단공간) 또는 projection space(투영공간) 변환 행렬
		DirectX::XMMATRIX modelViewProj;
		// local space
		DirectX::XMMATRIX model;
	};
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
	void Bind(Graphics& gfx) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	const Drawable& parent;
};