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
	// ���⼭ parent.GetTransformXM() �Լ��� ������Ʈ�� ������ ����� ����
	// world space�� ��ȯ�ϴ� ��ȯ ��Ʈ������ ��ȯ�Ѵ�.
	// ī�޶���� ���ϸ� view matrix �ϼ�
	const auto modelView = parent.GetTransformXM() * gfx.GetCamera();
	// ������۰� ���̴��� ������ �ϹǷ� transpose �ϰ� �ִ�.
	const Transforms tf =
	{
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView *
			gfx.GetProjection()
		)
	};
	pVcbuf->Update(gfx, tf);
	// ������۸� vs�� bind�Ѵ�.
	pVcbuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::pVcbuf;
