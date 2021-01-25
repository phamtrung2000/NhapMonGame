#pragma once
#include "GameObject.h"
class Block : public CGameObject
{
public:
	Block(int W, int H);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};

