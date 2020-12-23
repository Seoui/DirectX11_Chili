#pragma once
#include "Vertex.h"
#include <vector>
#include <DirectXMath.h>

class IndexedTriangleList
{
public:
	// 기본 생성자
	IndexedTriangleList() = default;

	/* verts_in  : 정점 배열(벡터), T는 정점의 data type
	   indices_in: 정점 index배열 */
	IndexedTriangleList(Dvtx::VertexBuffer verts_in, std::vector<unsigned short> indices_in)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in))
	{
		// 하나의 삼각형은 최소 3개의 점이 필요하다
		assert(vertices.Size() > 2);
		// 모든 삼각형의 정점의 개수는 3의 배수이어야 한다
		assert(indices.size() % 3 == 0);
	}

	/* !주의! 이 함수를 사용하려면 정점 자료구조에 pos attribute가 있어야 한다.
		v.pos는 각 정점의 local좌표기준 position이다.
		모든 정점을 월드 좌표 기준으로 변환한다. */
	void Transform(DirectX::FXMMATRIX matrix)
	{
		using Elements = Dvtx::VertexLayout::ElementType;
		for (int i = 0; i < vertices.Size(); i++)
		{
			auto& pos = vertices[i].Attr<Elements::Position3D>();
			DirectX::XMStoreFloat3(
				&pos,
				DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&pos), matrix)
			);
		}
	}
	void SetNormalIndependentFlat() noxnd
	{
		using namespace DirectX;
		using Type = Dvtx::VertexLayout::ElementType;
		// 세 점마다, 즉 하나의 삼각형마다
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			auto v0 = vertices[indices[i]];
			auto v1 = vertices[indices[i + 1]];
			auto v2 = vertices[indices[i + 2]];
			// p0, p1, p2는 XMVECTOR
			const auto p0 = XMLoadFloat3(&v0.Attr<Type::Position3D>());
			const auto p1 = XMLoadFloat3(&v1.Attr<Type::Position3D>());
			const auto p2 = XMLoadFloat3(&v2.Attr<Type::Position3D>());
			// XMVECTOR로 계산, 삼각형에서 두 변의 벡터를 외적 후 노멀라이즈하여 유닛노멀벡터를 구한다.
			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

			XMStoreFloat3(&v0.Attr<Type::Normal>(), n);
			XMStoreFloat3(&v1.Attr<Type::Normal>(), n);
			XMStoreFloat3(&v2.Attr<Type::Normal>(), n);
		}
	}

public:
	Dvtx::VertexBuffer vertices;
	std::vector<unsigned short> indices;
};