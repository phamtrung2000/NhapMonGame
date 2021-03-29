#pragma once
#include "Sprites.h"
#include<fstream>
#include <string>
#include"GameObject.h"
#include "Textures.h"

#define HUD_SECTION_UNKNOWN -1
#define HUD_SECTION_TEXTURES 2
#define HUD_SECTION_SPRITES 3
#define HUD_SECTION_ANIMATIONS 4
#define MAX_HUD_LINE 1024

#define COIN_SCORE 50
#define QUESTIONBRICKITEM_SCORE 1000


#define HEIGHT_SCORE_BROAD 28
#define ID_TEX_HUB 41

#define CAM_Y_HUD_ITEM	_Camera->cam_y + (SCREEN_HEIGHT * 3 / 4) - 10
#define CAM_Y_HUD_ITEM1	_Game->GetCamY() + (SCREEN_HEIGHT * 3 / 4) - 10

class HUD
{
	static HUD* __instance;
	ID3DXFont* font;
	RECT rect;
	string information;
	LPSPRITE Item1;
	LPSPRITE Item2;
	LPSPRITE Item3;
	LPSPRITE typePlayer;
	LPSPRITE HUB;
	LPSPRITE speed;
	int NumSpeed;
	LPSPRITE push;
	bool isDrawPush;
	float CamX, CamY;
public:
	bool hasItem1, hasItem2, hasItem3;
	int TypeItem[3];

	int Score, MarioLife, Money;
	int PlayTime; // thời gian chơi : 300s
	ULONGLONG countPlayTime; // biến đếm GetTickCount64 để trừ thời gian chơi đi 1

	static HUD* GetInstance();
	HUD();
	void LoadHUD(wstring);
	void Init();
	void Init(int playtime);
	void Update(DWORD dt);
	void Render();
	void Unload();
	~HUD();

	void UpdateItem(int _type);

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void UpdateScore(LPGAMEOBJECT e, int _nScore);
};

