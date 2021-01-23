#pragma once
#include "GameObject.h"

class WarpPipe : public CGameObject
{
public:
	bool HiddenWorld;
	int Color;
	int SceneID;
public:
	WarpPipe(int W, int H);
	WarpPipe(int W, int H,bool);
	WarpPipe(int W, int H, int, int);
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};

