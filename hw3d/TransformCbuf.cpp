#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
	:
	parent(parent)
{
	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	// 여기서 parent.GetTransformXM() 함수는 오브젝트의 완전히 계산이 끝난
	// world space로 변환하는 변환 매트릭스를 반환한다.
	// 카메라까지 곱하면 view matrix 완성
	const auto modelView = parent.GetTransformXM() * gfx.GetCamera();
	// 상수버퍼고 셰이더에 보내야 하므로 transpose 하고 있다.
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
	pVcbuf->Update(gfx, tf);
	// 상수버퍼를 vs에 bind한다.
	pVcbuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;
