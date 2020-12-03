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
			GPU가 정점들의 배열에 접근하려면, 그 정점들을 버퍼라고 부르는 GPU 자원에 넣어 두어야 한다.
			정점들을 저장하는 버퍼를 정점 버퍼(Vertex buffer)라고 부른다. 응용 프로그램에서 정점 같은
			자료 원소들의 배열을 GPU에 제공해야 할 때에는 항상! 버퍼를 사용한다.
			다음은 정점 버퍼를 만들기 전에 Description을 작성한다.
		*/
		D3D11_BUFFER_DESC bd = {};
		// 버퍼가 무엇으로써 어떻게 파이프라인에 바인드 될 것인지 설정
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// 자료 원소들이(정점) GPU나 CPU로부터 읽기, 쓰기 권한을 설정
		bd.Usage = D3D11_USAGE_DEFAULT;
		// 버퍼에 대해 CPU 접근 플래그
		bd.CPUAccessFlags = 0u;
		// 여러 종류의 플래그
		bd.MiscFlags = 0u;
		// 버퍼의 bytes 크기. 모든 정점들의 bytes 크기와 같다.
		bd.ByteWidth = UINT(sizeof(V) * vertices.size());
		// 정점이 배열안에서 어떻게 구분되어질지, 즉 정점배열에서 각 정점의 크기가 얼마인지.
		// GPU가 버퍼를 읽을 때 주어진 byte 크기 단위로 건너뛰면서 자료를 읽게 된다.
		bd.StructureByteStride = sizeof(V);
		// Subresource 데이터를 초기화하기 위한 데이터를 지정한다.
		// GPU에서 사용할 Subresource 데이터는 메모리에 올려진 데이터로 초기화해야한다.
		// 즉 정점 배열은 우리가 작성한 실제 값들이 들어있으며, 
		// 그 값들로 Subresource 데이터를 초기화한다.
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		// 모든 준비가 되었다면 GPU가 읽을 수 있는(또는 쓸 수도 있는) 정점 버퍼를 만든다.
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
