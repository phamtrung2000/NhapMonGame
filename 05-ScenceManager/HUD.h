#pragma once
#include "Sprites.h"
#include<fstream>
#include <string>


#define HUD_SECTION_UNKNOWN -1
#define HUD_SECTION_TEXTURES 2
#define HUD_SECTION_SPRITES 3
#define HUD_SECTION_ANIMATIONS 4
#define MAX_HUD_LINE 1024

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
	int Score, MarioLife, Money;
	ULONGLONG PlayTime, // thời gian chơi : 300s
		countPlayTime; // biến đếm GetTickCount64 để trừ thời gian chơi đi 1

	static HUD* GetInstance();
	HUD();
	void LoadHUD(wstring);
	void Init();
	void Update(float dt);
	void Render();
	void Unload();
	~HUD();

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
};

