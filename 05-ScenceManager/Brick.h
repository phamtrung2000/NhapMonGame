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
	Brick();
	Brick(int type);
	
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};