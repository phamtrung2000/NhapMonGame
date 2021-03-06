#pragma once
#include "GameObject.h"

#define BIRCK_NORMAL	0
#define BRICK_CLOUD	1

class Brick : public CGameObject
{
public:
	int Type;
	Brick();
	Brick(int type);
	
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};