#pragma once
#include "GameObject.h"
class Block : public CGameObject
{
public:
	int Width;
	int Height;
	Block(int W, int H)
	{
		ObjType = 6;
		Width = W;
		Height = H;
	}
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom)
	{
		left = x;
		top = y;
		right = left + Width;
		bottom = top + Height;
	}
	virtual void Render();
};

