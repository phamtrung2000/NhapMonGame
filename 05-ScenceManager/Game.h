#pragma once

#include <unordered_map>
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <iostream>
#include <fstream>

#include "Scence.h"
#include "Utils.h"

#include "Mario.h"
#include "MarioOverWorld.h"
#include "Game.h"
#include "Animations.h"
#include "HUD.h"
#include "Map.h"
#include "Camera.h"
#include "PlayScence.h"
#include "World1.h"
#include"Grid.h"

using namespace std;

#define DIRECTINPUT_VERSION 0x0800

#define OpeningSceneText "OpeningScene.txt"
#define OpeningSceneID	0
#define World1SceneText  ".\\textures\\Overworld\\World1Scene.txt"
#define World1SceneID	1
//#define Scene1_1Text  ".\\textures\World\\World1\\Scene1.1.txt"
#define Scene1_1Text  ".\\textures\World\\World1\\Scene1.4.txt"
#define Scene1_1ID		10
//#define SceneHidden1_1Text  ".\\textures\World\\World1\\SceneHidden1.1.txt"
#define SceneHidden1_1Text  ".\\textures\World\\World1\\SceneHidden1.4.txt"


#define SceneHidden1_1ID	11

#define KEYBOARD_BUFFER_SIZE 1024
#define SCREEN_WIDTH	300//400 
#define SCREEN_HEIGHT	260//280

#define _Mario Mario::GetInstance()
#define _MarioOverworld MarioOverWorld::GetInstance()
#define _Game  CGame::GetInstance()
#define _AnimationsSets CAnimationSets::GetInstance()
#define _HUD HUD::GetInstance()
#define _Map Map::GetInstance()
#define _Camera Camera::GetInstance()
#define _PlayScene CPlayScene::GetInstance()
#define _Grid Grid::GetInstance()

class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle
	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object
	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 
	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 
	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data
	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;
	int screen_width;
	int screen_height; 
	unordered_map<int, LPSCENE> scenes;
	

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:
	int current_scene;
	int MarioLife;
	float X_MarioOverworld, Y_MarioOverworld;
	int Score,Money;
	ULONGLONG PlayTime;
	bool DoneLoad;
	int MarioLevel;

	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);
	void SwitchScene2(int scene_id); // hàm chuyển cảnh nhưng thay đổi tọa độ mario thay vì khởi tạo như file scnene.txt ở world map
	void SwitchScene3(int scene_id, int level);
	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }
	float GetCamX() { return cam_x; }
	float GetCamY() { return cam_y; }
	void GetCamPos(float& x, float& y) { x = cam_x; y = cam_y; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }

	static CGame * GetInstance();

	~CGame();
};


