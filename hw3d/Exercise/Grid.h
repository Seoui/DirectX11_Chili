#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <initializer_list>

class Grid
{
public:
	template<class V>
	static IndexedTriangleList<V> Make(float width, float depth, unsigned int m, unsigned int n)
	{
		using UINT = unsigned int;
		namespace dx = DirectX;

		// m�� z��, n�� x��
		// ������ ����
		UINT vertexCount = m * n;
		// �ﰢ�� ���� ����
		UINT faceCount = (m - 1) * (n - 1) * 2;
		constexpr float side = 1.0f / 2.0f;	// 0.5f

		// �������� �����
		float halfWidth = 0.5f * width;	// �ʺ��� ����
		float halfDepth = 0.5f * depth;	// ������ ����

		float d_x = width / (n - 1);	// ���� �ϳ��� �ʺ�	����
		float d_z = depth / (m - 1);	// ���� �ϳ��� ���� ����

		float d_u = 1.0f / (n - 1);		// ���� �ϳ��� �ʺ� ����(uv��ǥ)
		float d_v = 1.0f / (m - 1);		// ���� �ϳ��� ���� ����(uv��ǥ)

		std::vector<V> vertices(vertexCount);
		for (UINT i = 0; i < m; ++i)
		{
			float z = halfDepth - i * d_z;
			for (UINT j = 0; j < n; ++j)
			{
				float x = -halfWidth + j * d_x;
				vertices[i * n + j].pos = { x, 0.0f, z };
			}
		}

		// ������ �ε��� 
		std::vector<unsigned short> indices(faceCount * 3);
		UINT k = 0;
		for (UINT i = 0; i < m - 1; ++i)
		{
			for (UINT j = 0; j < n - 1; ++j)
			{
				indices[k] = i * n + j;
				indices[k + 1] = i * n + j + 1;
				indices[k + 2] = (i + 1) * n + j;
				indices[k + 3] = (i + 1) * n + j;
				indices[k + 4] = i * n + j + 1;
				indices[k + 5] = (i + 1) * n + j + 1;
			}
		}
		return{ std::move(vertices), std::move(indices) };
	}
};