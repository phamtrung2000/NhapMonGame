#pragma once
#include "GameObject.h"
class Ground :
	public CGameObject
{
public:
	int Width;
	int Height;
	Ground(int W, int H)
	{
		Width = W;
		Height = H;
		ObjType = 7;
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

