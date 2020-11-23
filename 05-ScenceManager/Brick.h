#pragma once
#include "GameObject.h"
#define NORMAL	0
#define CLOUD	1

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class Brick : public CGameObject
{
public:
	int Type;
	Brick()
	{
		ObjType = 1;
		vx = vy = 0.f;
		Type = -1;
	}
	Brick(int type)
	{
		ObjType = 1;
		vx = vy = 0.f;
		Type = type;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};