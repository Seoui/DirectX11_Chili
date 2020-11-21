#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"
#include <DirectXMath.h>

// ��ȯ ��� ����
class TransformCbuf : public Bindable
{
private:
	struct Transforms
	{
		// homogeneous clip space(�������ܰ���) �Ǵ� projection space(��������) ��ȯ ���
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