/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 05 - SCENCE MANAGER

	This sample illustrates how to:

		1/ Implement a scence manager 
		2/ Load scene from "database", add/edit/remove scene without changing code 
		3/ Dynamically move between scenes without hardcode logic 
		
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Utils.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"

#include "PlayScence.h"

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"SAMPLE 05 - SCENCE MANAGER"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 250

#define MAX_FRAME_RATE 120

CGame *game;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	CGame::GetInstance()->GetCurrentScene()->Update(dt);
}

/*
	Render a frame 
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		CGame::GetInstance()->GetCurrentScene()->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount64();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount64();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();
			
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
 {
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);
	game->InitKeyboard();

	game->Load(L"mario-sample.txt");

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH*2, SCREEN_HEIGHT*2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}

//1	0	150	2
//1	16	150	2
//1	32	150	2
//1	48	150	2
//1	64	150	2
//1	80	150	2
//1	96	150	2
//1	112	150	2
//1	128	150	2
//1	144	150	2
//1	160	150	2
//1	176	150	2
//1	192	150	2
//1	208	150	2
//1	224	150	2
//1	240	150	2
//1	256	150	2
//1	272	150	2
//1	288	150	2
//1	304	150	2
//1	320	150	2
//1	336	150	2
//1	352	150	2
//1	368	150	2
//1	384	150	2
//1	400	150	2
//1	416	150	2
//1	432	150	2
//1	448	150	2
//1	464	150	2
//1	480	150	2
//1	496	150	2
//1	512	150	2
//1	528	150	2
//1	544	150	2
//1	560	150	2
//1	576	150	2
//1	592	150	2
//1	608	150	2
//1	624	150	2
//1	640	150	2
//1	656	150	2
//1	672	150	2
//1	688	150	2
//1	704	150	2
//1	720	150	2
//1	736	150	2
//1	752	150	2
//1	768	150	2
//1	784	150	2
//1	800	150	2
//1	816	150	2
//1	832	150	2
//1	848	150	2
//1	864	150	2
//1	880	150	2
//1	896	150	2
//1	912	150	2
//1	928	150	2
//1	944	150	2
//1	960	150	2
//1	976	150	2
//1	992	150	2
//1	1008	150	2
//1	1024	150	2
//1	1040	150	2
//1	1056	150	2
//1	1072	150	2
//1	1088	150	2
//1	1104	150	2
//1	1120	150	2
//1	1136	150	2
//1	1152	150	2
//1	1168	150	2
//1	1184	150	2
//1	1200	150	2
//1	1216	150	2
//1	1232	150	2
//1	1248	150	2
//1	1264	150	2
//1	1280	150	2
//1	1296	150	2
//1	1312	150	2