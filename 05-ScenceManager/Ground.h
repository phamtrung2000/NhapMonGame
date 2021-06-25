#pragma once
#include "GameObject.h"
class Ground :
	public CGameObject
{
public:
	Ground(int W, int H)
	{
		Width = W;
		Height = H;
		ObjType = 7;
		Category = CATEGORY::OBJECT;
	}
	
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};

