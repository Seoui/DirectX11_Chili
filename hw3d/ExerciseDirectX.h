#pragma once
#include <Window.h>
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>
using namespace DirectX;
namespace wrl = Microsoft::WRL;

class ExerciseDirectX
{
public:
	ExerciseDirectX();
	~ExerciseDirectX();

	void Section4Solution(wrl::ComPtr<ID3D11Device> pDevice);
	void Section5Solution();
private:

};

