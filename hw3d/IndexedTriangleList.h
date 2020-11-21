#pragma once
#include <vector>
#include <DirectXMath.h>

template<class T>
class IndexedTriangleList
{
public:
	// 기본 생성자
	IndexedTriangleList() = default;

	/*
		verts_in: 정점 배열(벡터), T는 정점의 data type
		indices_in: 정점 index배열 
	*/
	IndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned short> indices_in)
		:
		vertices(std::move(verts_in)),
		indices(std::move(indices_in))
	{
		// 하나의 삼각형은 최소 3개의 점이 필요하다
		assert(vertices.size() > 2);
		// 모든 삼각형의 정점의 개수는 3의 배수이어야 한다
		assert(indices.size() % 3 == 0);
	}

	/* 
		!주의! 이 함수를 사용하려면 정점 자료구조에 pos attribute가 있어야 한다.
		v.pos는 각 정점의 local좌표기준 position이다.
		모든 정점을 월드 좌표 기준으로 변환한다.
	*/
	void Transform(DirectX::FXMMATRIX matrix)
	{
		for (auto& v : vertices)
		{
			const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v.pos);
			DirectX::XMStoreFloat3(
				&v.pos,
				DirectX::XMVector3Transform(pos, matrix)
			);
		}
	}
	// asserts face-independent vertices w/ normals cleared to zero
	// 모든 삼각형 면에 수직인 노멀벡터를 구한다. 유닛벡터로 정규화까지 한다.
	void SetNormalsIndependentFlat() noexcept(!IS_DEBUG)
	{
		using namespace DirectX;
		// 정점의 개수는 3의 배수이고, 0개보다 많다
		assert(indices.size() % 3 == 0 && indices.size() > 0);
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			auto& v0 = vertices[indices[i]];
			auto& v1 = vertices[indices[i + 1]];
			auto& v2 = vertices[indices[i + 2]];
			const auto p0 = XMLoadFloat3(&v0.pos);
			const auto p1 = XMLoadFloat3(&v1.pos);
			const auto p2 = XMLoadFloat3(&v2.pos);

			// 삼각형 면에 수직인 노멀벡터를 구한다. 정규화까지
			// p1 - p0 = v, p2 - p0 = u 이면,
			// v x u = w, w는 v,u에 수직인 벡터이다.
			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

			// 삼각형의 각 꼭짓점의 노멀벡터는 면의 노멀벡터와 같다
			XMStoreFloat3(&v0.n, n);
			XMStoreFloat3(&v1.n, n);
			XMStoreFloat3(&v2.n, n);
		}
	}

public:
	std::vector<T> vertices;
	std::vector<unsigned short> indices;
};