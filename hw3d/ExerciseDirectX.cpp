#include "ExerciseDirectX.h"
#include <iostream>
#include <string>
#include <sstream>


#pragma comment(lib,"d3d11.lib")
ExerciseDirectX::ExerciseDirectX()
{
}
ExerciseDirectX::~ExerciseDirectX()
{
}

void ExerciseDirectX::Section4Solution(wrl::ComPtr<ID3D11Device> pDevice)
{
	wrl::ComPtr<IDXGIDevice1> dxgiDevice = 0;
	pDevice->QueryInterface(__uuidof(IDXGIDevice1), &dxgiDevice);

	wrl::ComPtr<IDXGIAdapter1> dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter1), &dxgiAdapter);

	wrl::ComPtr<IDXGIFactory1> dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), &dxgiFactory);

	DXGI_ADAPTER_DESC1 adapterDesc;
	UINT adapter_id = 0;
	OutputDebugStringW(L"************\n");
	while (dxgiFactory->EnumAdapters1(adapter_id, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		adapter_id++;
		dxgiAdapter->GetDesc1(&adapterDesc);

		OutputDebugStringW(adapterDesc.Description);
		OutputDebugStringW(L"\n");
	}
	OutputDebugStringW(L"************\n");
	UINT output_id = 0;
	wrl::ComPtr<IDXGIOutput> dxgiOutput = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter1), (void**)&dxgiAdapter);
	while (dxgiAdapter->EnumOutputs(output_id, &dxgiOutput) != DXGI_ERROR_NOT_FOUND)
	{
		output_id++;
		std::wstring number = std::to_wstring(output_id);
		OutputDebugStringW(number.c_str());
	}
	OutputDebugStringW(L"\n************\n");

	UINT mode_id = 0;
	dxgiAdapter->EnumOutputs(0, &dxgiOutput);
	dxgiOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &mode_id, 0);
	DXGI_MODE_DESC* modeDesc = new DXGI_MODE_DESC[mode_id];
	dxgiOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &mode_id, modeDesc);
	for (UINT i = 0; i < mode_id; i++)
	{
		std::wstring width = std::to_wstring(modeDesc[i].Width);
		std::wstring height = std::to_wstring(modeDesc[i].Height);
		std::wstring refreshNumerator = std::to_wstring(modeDesc[i].RefreshRate.Numerator);
		std::wstring refreshDenominator = std::to_wstring(modeDesc[i].RefreshRate.Denominator);
		OutputDebugStringW(L"WIDTH = ");
		OutputDebugStringW(width.c_str());
		OutputDebugStringW(L" HEIGHT = ");
		OutputDebugStringW(height.c_str());
		OutputDebugStringW(L" REFRESH = ");
		OutputDebugStringW(refreshNumerator.c_str());
		OutputDebugStringW(L"/");
		OutputDebugStringW(refreshDenominator.c_str());
		OutputDebugStringW(L"\n************\n");
	}
	OutputDebugStringW(L"\n************\n");

	/*	4.7 연습문제
	vp.TopLeftX = 100.0f;
	vp.TopLeftY = 100.0f;
	vp.Width = 500.0f;
	vp.Height = 400.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	*/
}

void ExerciseDirectX::Section5Solution()
{
	FXMVECTOR eye = XMVectorSet(-20.0f, 35.0f, -50.0f, 1.0f);
	FXMVECTOR target = XMVectorSet(10.0f, 0.0f, 30.0f, 1.0f);
	FXMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX ViewMatrix = XMMatrixLookAtLH(eye, target, up);
	OutputDebugStringW(L"************View Matrix 4번 문제*************\n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::wstring number = std::to_wstring(ViewMatrix.m[i][j]);
			OutputDebugStringW(number.c_str());
			OutputDebugStringW(L", ");
		}
		OutputDebugStringW(L"\n");
	}
	OutputDebugStringW(L"******************************************\n");

	XMMATRIX mat = XMMatrixPerspectiveFovLH(0.25f*XM_PI, 1.3333f, 1.0f, 100.0f);
	OutputDebugStringW(L"***************Projection Matrix 5번 문제***************\n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::wstring number = std::to_wstring(mat.m[i][j]);
			OutputDebugStringW(number.c_str());
			OutputDebugStringW(L", ");
		}
		OutputDebugStringW(L"\n");
	}
	OutputDebugStringW(L"******************************************\n");
	/*
	float dgree = 45.0f;
	float rad = XMConvertToRadians(45.0f);
	float cosValue = XMScalarCos(rad / 2);
	float sinValue = XMScalarSin(rad / 2);
	float tanValue = sinValue / cosValue;
	float ret1 = 3 / (4 * tanValue);
	float ret2 = 1 / tanValue;
	OutputDebugStringW(L"***************R  e  s  u  l  t******************\n");
	std::wstring number = std::to_wstring(ret1);
	OutputDebugStringW(number.c_str());
	OutputDebugStringW(L", ");
	number = std::to_wstring(ret2);
	OutputDebugStringW(number.c_str());
	OutputDebugStringW(L"\n");
	OutputDebugStringW(L"******************************************\n");
	*/
	float rad = XMConvertToRadians(30.0f);
	XMMATRIX mat6 = XMMatrixPerspectiveFovLH(rad, 2.0f, 5.0f, 200.0f);
	OutputDebugStringW(L"***************6번 문제***************\n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::wstring number = std::to_wstring(mat6.m[i][j]);
			OutputDebugStringW(number.c_str());
			OutputDebugStringW(L", ");
		}
		OutputDebugStringW(L"\n");
	}
	OutputDebugStringW(L"******************************************\n");
}