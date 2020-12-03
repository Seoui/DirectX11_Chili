#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include "Vertex.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
		:
		stride(sizeof(V))
	{
		INFOMAN(gfx);

		/*
			GPU�� �������� �迭�� �����Ϸ���, �� �������� ���۶�� �θ��� GPU �ڿ��� �־� �ξ�� �Ѵ�.
			�������� �����ϴ� ���۸� ���� ����(Vertex buffer)��� �θ���. ���� ���α׷����� ���� ����
			�ڷ� ���ҵ��� �迭�� GPU�� �����ؾ� �� ������ �׻�! ���۸� ����Ѵ�.
			������ ���� ���۸� ����� ���� Description�� �ۼ��Ѵ�.
		*/
		D3D11_BUFFER_DESC bd = {};
		// ���۰� �������ν� ��� ���������ο� ���ε� �� ������ ����
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// �ڷ� ���ҵ���(����) GPU�� CPU�κ��� �б�, ���� ������ ����
		bd.Usage = D3D11_USAGE_DEFAULT;
		// ���ۿ� ���� CPU ���� �÷���
		bd.CPUAccessFlags = 0u;
		// ���� ������ �÷���
		bd.MiscFlags = 0u;
		// ������ bytes ũ��. ��� �������� bytes ũ��� ����.
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		// ������ �迭�ȿ��� ��� ���еǾ�����, �� �����迭���� �� ������ ũ�Ⱑ ������.
		// GPU�� ���۸� ���� �� �־��� byte ũ�� ������ �ǳʶٸ鼭 �ڷḦ �а� �ȴ�.
		bd.StructureByteStride = sizeof(V);
		// Subresource �����͸� �ʱ�ȭ�ϱ� ���� �����͸� �����Ѵ�.
		// GPU���� ����� Subresource �����ʹ� �޸𸮿� �÷��� �����ͷ� �ʱ�ȭ�ؾ��Ѵ�.
		// �� ���� �迭�� �츮�� �ۼ��� ���� ������ ���������, 
		// �� ����� Subresource �����͸� �ʱ�ȭ�Ѵ�.
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		// ��� �غ� �Ǿ��ٸ� GPU�� ���� �� �ִ�(�Ǵ� �� ���� �ִ�) ���� ���۸� �����.
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}
	VertexBuffer(Graphics& gfx, const hw3dexp::VertexBuffer& vbuf)
		:
		stride((UINT)vbuf.GetLayout().Size())
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(vbuf.SizeBytes());
		bd.StructureByteStride = stride;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vbuf.GetData();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}
	void Bind(Graphics& gfx) noexcept override;
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
