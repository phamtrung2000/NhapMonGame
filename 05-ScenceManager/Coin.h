#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH  8
#define COIN_BBOX_HEIGHT 16
#define COIN_ANI		 0
class Coin : public CGameObject
{
public:
	Coin();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};