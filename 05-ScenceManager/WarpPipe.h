#pragma once
#include "GameObject.h"
#define TileSize	16

#define Ani_Start_Right		0
#define Ani_Start_LEFT		1

#define Ani_Body_Left		2
#define Ani_Body_Right		3

class WarpPipe : public CGameObject
{
public:
	int Width;
	int Height;
	bool HiddenWorld;

public:
	WarpPipe(int W, int H);
	WarpPipe(int W, int H,bool);
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};

