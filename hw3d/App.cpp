#include "App.h"
#include <memory>
#include <algorithm>
#include "ChiliMath.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "VertexBuffer.h"

namespace dx = DirectX;
GDIPlusManager gdipm;
/*
	r		: 거리
	theta	: 수평시야각
	phi		: 수직시야각
*/
App::App()
	:
	// 1280 : 720  = 16 : 9
	wnd(1280, 720, "The Donkey Fart Box"),
	light(wnd.Gfx()),
	plane(wnd.Gfx(), 3.0f),
	cube(wnd.Gfx(), 4.0f)
{
	plane.SetPos({ 1.0f, 17.0f, -1.0f });
	cube.SetPos({ 3.0f, 14.0f, -2.0f });
	// XMMatrixPerspectiveLH(수직시야각, 종횡비, 가까운 평면까지 거리, 먼 평면까지 거리);
	// 주의! 아래 두 줄의 코드는 Graphics 클래스에서 하나의 Matrix인 변수에다가 Setting 해준 것일 뿐이다.
	// 실제 카메라는 cam 클래스 변수이다.
	// 프로젝션은 안변한다, 생성자에 한번 설정하고 계속 간다.
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 40.0f));
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	/*
		BeginFrame은 그려질 준비를 한다. 
		주어진 색상으로
		RenderTargetView와 DepthStencilView를 Clear 한다
	*/
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	/* 
		여기서 비로소 cam의 view matrix(rotation matrix를 곱한)를 셋팅한다.
		cam 클래스 내부에서 위치와 회전을 설정한다.
		graphics 클래스에 cam의 view matrix를 설정하는 이유는 다른 오브젝틀과 통신하기 위함이다
		DoFrame() 함수는 가장 높은 레벨의 반복문 함수안에 있다.
		(windows 클래스 제외 가장 먼저 실행되는 함수라는 의미)
	*/
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());

	nano.Draw(wnd.Gfx());
	//nano2.Draw(wnd.Gfx());
	light.Draw(wnd.Gfx());
	plane.Draw(wnd.Gfx());
	cube.Draw(wnd.Gfx());
	
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		}
	}

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('W'))
		{
			cam.Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cam.Translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('R'))
		{
			cam.Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('F'))
		{
			cam.Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{ 
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}

	// imgui windows
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowImguiDemoWindow();
	nano.ShowWindow("Model 1");
	//nano2.ShowWindow("Model 2");
	plane.SpawnControlWindow(wnd.Gfx());
	cube.SpwanControlWindow(wnd.Gfx());

	// present
	// 실제로 화면에 표현
	wnd.Gfx().EndFrame();
}

void App::ShowImguiDemoWindow()
{
	if (showDemoWindow)
	{
		ImGui::ShowDemoWindow(&showDemoWindow);
	}
}

App::~App()
{}

int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}